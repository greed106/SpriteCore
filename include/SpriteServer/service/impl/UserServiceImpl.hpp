#pragma once

#include "SpriteServer/service/UserService.hpp"
#include "SpriteServer/mapper/UserMapper.hpp"

class UserServiceImpl : public UserService {
public:
    static UserServiceImpl& getInstance() {
        static UserServiceImpl instance;
        return instance;
    }

    void addUser(const std::string& name, const std::string& password) override {
        User user(name, password);
        userMapper.addUser(user);
    }

    bool login(const std::string& name, const std::string& password) override {
        auto user = userMapper.getUserByName(name);
        if (user == nullptr) {
            return false;
        }
        return user->checkPassword(password);
    }

    std::shared_ptr<User> getUser(const std::string& name) override {
        return userMapper.getUserByName(name);
    }

private:
    UserMapper& userMapper;

    UserServiceImpl() : userMapper(UserMapper::getInstance()) {}
    ~UserServiceImpl() = default;

    UserServiceImpl(const UserServiceImpl&) = delete;
    UserServiceImpl& operator=(const UserServiceImpl&) = delete;
};
