#pragma once

#include "State.hpp"

class ExitState : public State {
public:
    ExitState() {
        commands["help"] = [](Context& context) {
            State::logger->info("Already exited. No commands are available.");
        };
    }
};