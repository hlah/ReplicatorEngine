#include "engine.hpp"

#include "state.hpp"
#include "object.hpp"
#include "matrix_op.hpp"
#include "hierarchy.hpp"
#include "time.hpp"

#include "entt/entt.hpp"

void Engine::run(State* state_ptr) {
    _running = true;

    spdlog::set_level(_loglevel);

    // create window
    auto window = std::make_shared<Window>( _title, _width, _height, _aa );
    entt::registry registry;
    registry.set<WindowHandler>( window );

    // watch for hierarchy changes
    registry.on_construct<Hierarchy>().connect<&Hierarchy::on_construct>();
    registry.on_destroy<Hierarchy>().connect<&Hierarchy::on_destroy>();
    // watch for transform changes
    registry.on_construct<Transform>().connect<&Transform::on_change>();
    registry.on_replace<Transform>().connect<&Transform::on_change>();

    // create default caches
    registry.set<entt::resource_cache<ShaderProgram>>();
    registry.set<entt::resource_cache<Texture>>();

    spdlog::info("Running!");
    
    window->poll_events();
    state_ptr->on_start( registry );

    double before = window->time();
    while( _running ) {
        window->poll_events();

        if( window->mouse_moved() ) {
            state_ptr->on_mouse_move( registry, window->mouse_x(), window->mouse_y() ) ;
        }
        _process_actions( registry, state_ptr, *window );

        window->clear();
        double now = window->time();
        registry.set<DeltaTime>( now - before );
        state_ptr->update( registry );
        before = now;

        window->refresh();
        window->reset();
    }
    spdlog::info("Stoped.");
}

void Engine::stop() {
    _running = false;
}

void Engine::set_window_size(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}

void Engine::set_window_title(const std::string& title) {
    _title = title;
}

ActionId Engine::get_action_id( const std::string& name ) {
    auto action_it = std::find_if( _actions.begin(), _actions.end(),
            [&name] (auto it) { return (it.second == name); }
    );
    if( action_it == _actions.end() ) {
        _actions[_next_action_id] = name;
        return _next_action_id++;
    }
    else {
        return action_it->first;
    }
}

void Engine::bind_button( std::variant<Key, MouseButton> button, const std::string& action_name ) {
    auto action = get_action_id( action_name );
    _bindings[button] = action;
}

void Engine::_process_actions( entt::registry& registry, State* state_ptr, Window& window ) {
    // check for close event
    if( window.should_close() ) {
        auto transition = state_ptr->on_close( registry );
        window.should_close(false);
        _process_transition( transition );
    }

    // process input events
    std::optional<std::pair<std::variant<Key, MouseButton>, InputEventType>> button_event;
    while( (button_event = window.next_button()) ) {
        if( _bindings.count( button_event->first ) != 0 ) {
            // Key press -> action ON
            if( button_event->second == InputEventType::Press ) {
                auto transition = state_ptr->on_action( 
                        registry,
                        ActionEvent{ ActionEvent::Type::ON, _actions.at( _bindings.at( button_event->first ) ) }
                );
                _process_transition( transition );
            } 
            // Key release -> action OFF
            else if( button_event->second == InputEventType::Release ) {
                auto transition = state_ptr->on_action( 
                        registry,
                        ActionEvent{ ActionEvent::Type::OFF, _actions.at( _bindings.at( button_event->first ) ) }
                );
                _process_transition( transition );
            } 
        }
    }
}

void Engine::_process_transition( State::Transition trans ) {
    switch( trans ) {
        case State::Transition::QUIT:
            stop();
            break;
        case State::Transition::NONE:
            break;
    }
}
