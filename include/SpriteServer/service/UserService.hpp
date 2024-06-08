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
    virtual void updateUser(const User& user) = 0;
    virtual void addMedal(const std::string& name, const std::string& medal) = 0;
    virtual void addWinner(const std::string& name) = 0;
    virtual void addBattleTimes(const std::string& name) = 0;
    virtual void setOnline(const std::string& name, bool online) = 0;
    virtual std::vector<std::string> getOnlineUserNames() = 0;
    virtual bool isUserNameExist(const std::string& username) = 0;
    virtual std::vector<std::string> getAllUserNames() = 0;
};