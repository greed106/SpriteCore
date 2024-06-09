#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "SpriteCore/Sprite.hpp"
#include "nlohmann/json.hpp"

class User {
private:
    std::string name;
    std::string password;
    int winner;
    int battleTimes;
    std::vector<std::string> medals;
    std::vector<Sprite> sprites;

public:
    User(std::string name, std::string password, int winner = 0, int battleTimes = 0)
            : name(name), password(password), winner(winner), battleTimes(battleTimes) {}

    void showInfo() const {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Battle Times: " << battleTimes << std::endl;
        std::cout << "Winner: " << winner << std::endl;
        std::cout << "Medals: ";
        for (auto medal : medals) {
            std::cout << medal << " ";
        }
        std::cout << std::endl;
        std::cout << "Sprites: " << std::endl;
        for (auto sprite : sprites) {
            sprite.showInfo();
        }
    }

    std::string getName() const {
        return name;
    }

    bool checkPassword(std::string password) const {
        return this->password == password;
    }

    std::string getPassword() const {
        return password;
    }

    double getRate() const {
        return winner * 1.0 / battleTimes;
    }

    void setPassword(std::string newPassword) {
        password = newPassword;
    }

    void setName(std::string newName) {
        name = newName;
    }

    void addMedal(std::string medal) {
        medals.push_back(medal);
    }

    void addSprite(Sprite sprite) {
        sprites.push_back(sprite);
    }

    void removeSprite(std::string spriteName) {
        for (auto it = sprites.begin(); it != sprites.end(); it++) {
            if (it->getSpriteName() == spriteName) {
                sprites.erase(it);
                break;
            }
        }
    }

    void showMedals() {
        for (auto medal : medals) {
            std::cout << medal << " ";
        }
        std::cout << std::endl;
    }

    static nlohmann::json toJson(const User& user) {
        nlohmann::json j;
        j["name"] = user.name;
        j["password"] = user.password;
        j["battleTimes"] = user.battleTimes;
        j["winner"] = user.winner;
        j["medals"] = user.medals;
        j["sprites"] = nlohmann::json::array();
        for (auto sprite : user.sprites) {
            j["sprites"].push_back(Sprite::toJson(sprite));
        }
        return j;
    }

    static std::shared_ptr<User> fromJson(const nlohmann::json& j) {
        std::string name = j["name"];
        std::string password = j["password"];
        int battleTimes = j["battleTimes"];
        int winner = j["winner"];
        std::vector<std::string> medals = j["medals"];
        std::vector<Sprite> sprites;
        for (auto sprite : j["sprites"]) {
            sprites.push_back(*Sprite::fromJson(sprite));
        }
        auto user = std::make_shared<User>(name, password, winner, battleTimes);
        user->setMedals(medals);
        user->setSprites(sprites);
        return user;
    }

    void setSprites(std::vector<Sprite> sprites) {
        this->sprites = sprites;
    }

    void setMedals(std::vector<std::string> medals) {
        this->medals = medals;
    }

    void setBattleTimes(int battleTimes) {
        this->battleTimes = battleTimes;
    }

    int getBattleTimes() const {
        return battleTimes;
    }

    void addWinner() {
        winner++;
        battleTimes++;
    }

    int getWinner() const {
        return winner;
    }

    void addBattleTimes() {
        battleTimes++;
    }
};
