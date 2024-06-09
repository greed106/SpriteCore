#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Context.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>

class State {
public:
    using CommandHandler = std::function<void(Context&)>;

    virtual void handle(Context& context, const std::string& command) {
        if (command == "help") {
            printCommands();
        } else {
            auto it = commands.find(command);
            if (it != commands.end()) {
                it->second(context);
            } else {
                logger->error("Unknown command: {}", command);
            }
        }
    }

    virtual void printCommands() const {
        std::cout << "Available commands: ";
        for (const auto& cmd : commands) {
            std::cout << cmd.first << " ";
        }
        std::cout << std::endl;
    }

    State() {
        // 仅在第一次创建时初始化logger
        if (!logger) {
            logger = spdlog::stdout_color_mt("State");
            logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");
        }
    }

protected:
    std::unordered_map<std::string, CommandHandler> commands;
    static std::shared_ptr<spdlog::logger> logger;

    static std::string inputStr(const std::string& prompt){
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }
};