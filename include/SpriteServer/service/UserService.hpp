#pragma once

#include <string>
#include "SpriteServer/pojo/User.hpp"

class UserService {
protected:
    UserService() = default;
    virtual ~UserService() = default;
public:
    UserService(const UserService&) = delete;
    UserService& operator=(const UserService&) = delete;

    virtual void addUser(const std::string& name, const std::string& password) = 0;
    virtual bool login(const std::string& name, const std::string& password) = 0;
    virtual std::shared_ptr<User> getUser(const std::string& name) = 0;
};