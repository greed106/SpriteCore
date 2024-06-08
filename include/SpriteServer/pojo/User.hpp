#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "SpriteCore/Sprite.hpp"

class User {
private:
    std::string name;
    std::string password;
    int winner;
    int battleTimes;
    std::vector<std::string> medals;
    std::vector<Sprite> sprites;
public:
    User(std::string name, std::string password, int winner = 0, int battleTimes = 0);
    void showInfo() const;
    std::string getName() const;
    std::string getPassword() const;
    double getRate() const;
    void setPassword(std::string newPassword);
    void setName(std::string newName);
    void setSprites(std::vector<Sprite> sprites);
    void setMedals(std::vector<std::string> medals);
    void addMedal(std::string medal);
    void addSprite(Sprite sprite);
    void removeSprite(std::string spriteName);
    void showMedals();
    bool checkPassword(std::string password) const;
    static nlohmann::json toJson(const User& user);
    static std::shared_ptr<User> fromJson(const nlohmann::json& j);
    void setBattleTimes(int battleTimes);
    int getBattleTimes() const;
    void addWinner();
    int getWinner() const;
    void addBattleTimes();
};