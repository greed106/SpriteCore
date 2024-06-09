#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <hv/requests.h>

class State;

class Context {
public:
    static Context& getInstance();
    void setState(std::shared_ptr<State> state);
    void request(const std::string& command);
    std::string getBaseUrl() const { return baseUrl; }
    std::string getUsername() const { return username; }
    void setUsername(const std::string& username) { this->username = username; }
    void setExit(bool isExit) { this->isExit = isExit; }
    bool getIsExit() const { return isExit; }
private:
    std::shared_ptr<State> currentState;
    std::shared_ptr<spdlog::logger> logger;
    std::string baseUrl = "http://localhost:8080";
    bool isExit = true;
    std::string username;

    Context();
};