#include "SpriteClient/AuthenticatedState.hpp"
#include "SpriteClient/UnauthenticatedState.hpp"
#include "SpriteClient/ExitState.hpp"

UnauthenticatedState::UnauthenticatedState() {
    commands["login"] = [](Context& context) {
        auto username = inputStr("username: ");
        auto password = inputStr("password: ");
        context.setUsername(username);
        auto url = context.getBaseUrl() + "/login";

        hv::Json json;
        json["username"] = username;
        json["password"] = password;

        auto resp = requests::post(url.c_str(), json.dump());
        if(resp == nullptr || !nlohmann::json::parse(resp->body)["valid"].get<bool>()){
            State::logger->error("Failed to login");
        }else{
            State::logger->info("Logged in as {} successfully", username);
            context.setState(std::make_shared<AuthenticatedState>());
            context.setExit(false);
        }
    };
    commands["exit"] = [](Context& context) {
        State::logger->info("Exiting...");
        context.setUsername("");
        context.setState(std::make_shared<ExitState>());
        context.setExit(true);
    };
    commands["register"] = [](Context& context) {
        auto username = inputStr("username: ");
        auto password = inputStr("password: ");
        auto url = context.getBaseUrl() + "/register";

        hv::Json json;
        json["username"] = username;
        json["password"] = password;

        auto resp = requests::post(url.c_str(), json.dump());
        if(resp == nullptr || !nlohmann::json::parse(resp->body)["valid"].get<bool>()){
            State::logger->error("Failed to register");
        }else{
            State::logger->info("Registered as {} successfully", username);
            context.setState(std::make_shared<AuthenticatedState>());
            context.setExit(false);
        }
    };
}