#include "SpriteClient/AuthenticatedState.hpp"
#include "SpriteClient/UnauthenticatedState.hpp"
#include "SpriteServer/pojo/DTO/BattleRequest.hpp"
#include "SpriteBattle/BattleResult.hpp"

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
    // 查看指定用户的信息
    commands["info"] = [](Context& context) {
        auto user = AuthenticatedState::getUserFromServer(context);
        if(user != nullptr) {
            logger->info("Get {}'s info successfully", user->getName());
            user->showInfo();
        }
    };
    // 查看自己的信息
    commands["my_info"] = [](Context& context) {
        auto user = AuthenticatedState::getUserFromServer(context, true);
        if(user != nullptr) {
            logger->info("Get {}'s info successfully", user->getName());
            user->showInfo();
        }
    };
    // 进行训练对战
    commands["pve_train"] = [](Context& context) {
        auto user = AuthenticatedState::getUserFromServer(context, true);
        State::logger->info("Please choose a sprite to train: ");
        user->showSpriteInfo();
        auto challenger = AuthenticatedState::getSpriteFromUser(user);
        auto computer = getUserFromServer(context, std::string("computer"));
        State::logger->info("Please choose computer's sprite:");
        computer->showSpriteInfo();
        auto opponent = AuthenticatedState::getSpriteFromUser(computer);
        nlohmann::json j = BattleRequest(context.getUsername(), *challenger, *challenger, true, opponent->getSpriteName());
        auto url = context.getBaseUrl() + "/battle";
        auto resp = requests::post(url.c_str(), j.dump());
        if(resp == nullptr || resp->status_code != HTTP_STATUS_OK)  State::logger->error("Failed to train");
        else    nlohmann::json::parse(resp->body).get<BattleResult>().showInfo();
    };
    // 进行决斗
    commands["pvp_battle"] = [](Context& context) {
        auto user = AuthenticatedState::getUserFromServer(context, true);
        State::logger->info("Please choose your sprite: ");
        user->showSpriteInfo();
        auto challenger = AuthenticatedState::getSpriteFromUser(user);
        State::logger->info("Please choose a sprite as prisoner: ");
        auto prisoner = AuthenticatedState::getRandomSpriteFromUser(user);
        nlohmann::json j = BattleRequest(context.getUsername(), *challenger, *prisoner, false, "");
        context.openWebSocket();
        context.sendMessage(j.dump());
        State::logger->info("Waiting for battle result...");
        auto res = context.getResult();
//        context.closeWebSocket();
        State::logger->info("Battle result: ");
        res.showInfo();
    };
    // 进行电脑对战
    commands["pve_battle"] = [](Context& context) {
        auto user = AuthenticatedState::getUserFromServer(context, true);
        State::logger->info("Please choose your sprite: ");
        user->showSpriteInfo();
        auto challenger = AuthenticatedState::getSpriteFromUser(user);
        State::logger->info("Please choose a sprite as prisoner: ");
        auto prisoner = AuthenticatedState::getRandomSpriteFromUser(user);
        auto computer = getUserFromServer(context, std::string("computer"));
        State::logger->info("Please choose computer's sprite:");
        computer->showSpriteInfo();
        auto opponent = AuthenticatedState::getSpriteFromUser(computer);
        nlohmann::json j = BattleRequest(context.getUsername(), *challenger, *prisoner, true, opponent->getSpriteName());
        context.openWebSocket();
        context.sendMessage(j.dump());
        auto res = context.getResult();
        State::logger->info("Battle result: ");
        res.showInfo();
    };


}

std::shared_ptr<User> AuthenticatedState::getUserFromServer(Context& context, bool isSelf) {
    auto username = isSelf ? context.getUsername() : inputStr("username: ");
    auto url = context.getBaseUrl() + "/user?name=" + username;
    auto resp = requests::get(url.c_str());
    if(resp == nullptr || resp->status_code != HTTP_STATUS_OK) {
        State::logger->error("Failed to get info");
    }else{
        auto j = nlohmann::json::parse(resp->body);
        auto user = User::fromJson(j);
        return user;
    }
    return nullptr;
}

std::shared_ptr<User> AuthenticatedState::getUserFromServer(Context &context, const std::string &username) {
    auto url = context.getBaseUrl() + "/user?name=" + username;
    auto resp = requests::get(url.c_str());
    if(resp == nullptr || resp->status_code != HTTP_STATUS_OK) {
        State::logger->error("Failed to get info");
    }else{
        auto j = nlohmann::json::parse(resp->body);
        auto user = User::fromJson(j);
        return user;
    }
    return nullptr;
}

std::shared_ptr<JsonSprite> AuthenticatedState::getSpriteFromUser(std::shared_ptr<User> user) {
    while(true){
        auto sprite = user->getSprite(inputStr("sprite name: "));
        if(sprite != nullptr)   return sprite;
        State::logger->error("Invalid sprite name");
    }
}

std::shared_ptr<JsonSprite> AuthenticatedState::getRandomSpriteFromUser(std::shared_ptr<User> user) {
    auto randomSprites = user->getRandomSpriteNames();
    State::logger->info(fmt::format("Random sprites: {}", randomSprites));
    auto name = inputStr("sprite name: ");
    while(std::find(randomSprites.begin(), randomSprites.end(), name) == randomSprites.end()){
        State::logger->error("Invalid sprite name");
        name = inputStr("sprite name: ");
    }
    return user->getSprite(name);
}