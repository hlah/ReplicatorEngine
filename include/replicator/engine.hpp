#ifndef _REPLICATOR_ENGINE_HPP_
#define _REPLICATOR_ENGINE_HPP_

#include "state.hpp"
#include "input.hpp"
#include "window.hpp"

#include "spdlog/spdlog.h"

#include <string>
#include <cmath>

class Engine {
    public:
        // Constructor
        Engine() : _running{false}, _width{512}, _height{512}, _title{} {}

        // run engine
        void run(State* state_ptr);

        // stop engine
        void stop();

        // Setters
        void set_window_size(unsigned int width, unsigned int height);
        void set_window_title(const std::string& title);
        void set_aa(unsigned int aa) { _aa = aa; };

        ActionId get_action_id( const std::string& name );
        void bind_button( std::variant<Key, MouseButton> button, const std::string& action_name );


    private:
        bool _running;
        unsigned int _width, _height, _aa = 0;
        std::string _title;

        ActionId _next_action_id = 0;
        std::unordered_map<ActionId, std::string> _actions;
        std::unordered_map<std::variant<Key, MouseButton>, ActionId> _bindings;

        // process actions
        void _process_actions( entt::registry& registry, State* state_ptr, Window& window );
        void _process_transition( State::Transition trans );

#ifdef DEBUG
        spdlog::level::level_enum _loglevel = spdlog::level::debug;
#else 
        spdlog::level::level_enum _loglevel = spdlog::level::info;
#endif


};

#endif // _REPLICATOR_ENGINE_HPP_
