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
        auto user = userMapper.getUserByName(name);
        if(user == nullptr) {
            std::cerr << "User not found: " + name << std::endl;
            return nullptr;
        }
        auto medals = userMapper.getMedalsByName(name);
        for(auto& medal : medals) {
            user->addMedal(medal);
        }
        return user;
    }

    void updateUser(const User& user) override {
        userMapper.updateUser(user);
    }

    void addMedal(const std::string& name, const std::string& medal) override {
        userMapper.addMedal(name, medal);
    }

    void addWinner(const std::string& name) override {
        auto user = userMapper.getUserByName(name);
        user->addWinner();
        userMapper.updateUser(*user);
    }

    void addBattleTimes(const std::string& name) override {
        auto user = userMapper.getUserByName(name);
        user->addBattleTimes();
        userMapper.updateUser(*user);
    }

    void setOnline(const std::string& name, bool online) override {
        userMapper.setOnline(name, online);
    }

    std::vector<std::string> getOnlineUserNames() override {
        auto users = userMapper.getOnlineUsers();
        std::vector<std::string> names;
        for(const auto& user : users){
            names.push_back(user->getName());
        }
        return names;
    }

    bool isUserNameExist(const std::string& username) override {
        auto user = userMapper.getUserByName(username);
        return user != nullptr;
    }

    std::vector<std::string> getAllUserNames() override{
        auto users = userMapper.getAllUsers();
        std::vector<std::string> userNames;
        for(auto& user : users) {
            userNames.push_back(user->getName());
        }
        return userNames;
    }
private:
    UserMapper& userMapper;

    UserServiceImpl() : userMapper(UserMapper::getInstance()) {}
    ~UserServiceImpl() = default;
};
