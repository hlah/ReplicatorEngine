#ifndef _REPLICATOR_STATE_HPP_
#define _REPLICATOR_STATE_HPP_

#include "world.hpp"
#include "action.hpp"

class State {
    public:
        // Type of transition for State
        enum class Transition {
            NONE,
            QUIT,
        };

        virtual Transition on_start(World& world) { return Transition::NONE; }
        virtual Transition update( World& world, double delta_time ) { return Transition::NONE; }
        virtual Transition on_close() { return Transition::QUIT; }
        virtual Transition on_action( const ActionEvent& action, World& world ) { return Transition::NONE; }
};



#endif // _REPLICATOR_STATE_HPP_
