#pragma once

#include "State.hpp"
#include "SpriteServer/pojo/User.hpp"

class AuthenticatedState : public State {
private:
    static std::shared_ptr<User> getUserFromServer(Context& context, bool isSelf = false);
    static std::shared_ptr<User> getUserFromServer(Context& context, const std::string& username);
    static std::shared_ptr<JsonSprite> getSpriteFromUser(std::shared_ptr<User> user);
    static std::shared_ptr<JsonSprite> getRandomSpriteFromUser(std::shared_ptr<User> user);
public:
    AuthenticatedState();
};