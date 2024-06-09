#include "SpriteClient/AuthenticatedState.hpp"
#include "SpriteClient/UnauthenticatedState.hpp"


AuthenticatedState::AuthenticatedState() {
    commands["logout"] = [](Context& context) {
        auto url = context.getBaseUrl() + "/exit?name=" + context.getUsername();
        auto resp = requests::get(url.c_str());
        if(resp == nullptr || !nlohmann::json::parse(resp->body)["valid"].get<bool>()) {
            State::logger->error("Failed to logout");
        }else{
            State::logger->info("Logged out");
            context.setUsername("");
            context.setState(std::make_shared<UnauthenticatedState>());
        }
    };
    // 查看所有在线用户
    commands["online_list"] = [](Context& context) {
        auto url = context.getBaseUrl() + "/online_user";
        auto resp = requests::get(url.c_str());
        if(resp == nullptr) {
            State::logger->error("Failed to list");
        }else{
            auto usernames = nlohmann::json::parse(resp->body)["names"].get<std::vector<std::string>>();
            State::logger->info(fmt::format("Online users: {}", usernames));
        }
    };
    // 查看所有用户
    commands["all_list"] = [](Context& context) {
        auto url = context.getBaseUrl() + "/all_user";
        auto resp = requests::get(url.c_str());
        if(resp == nullptr) {
            State::logger->error("Failed to list");
        }else{
            auto usernames = nlohmann::json::parse(resp->body)["names"].get<std::vector<std::string>>();
            State::logger->info(fmt::format("All users: {}", usernames));
        }
    };
}