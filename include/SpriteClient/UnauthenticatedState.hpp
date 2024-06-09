#pragma once

#include "State.hpp"

class UnauthenticatedState : public State {
private:
    static std::string inputStr(const std::string& prompt){
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }
public:
    UnauthenticatedState();
};