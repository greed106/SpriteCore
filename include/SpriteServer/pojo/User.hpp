#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "SpriteCore/Sprite.hpp"
#include "nlohmann/json.hpp"
#include "JsonSprite.hpp"
#include <fmt/core.h>
#include <fmt/color.h>
#include <random>
#include <algorithm>

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
        fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "User Information:\n");
        fmt::print(fmt::fg(fmt::color::yellow), "Name: {}\n", name);
//        fmt::print(fmt::fg(fmt::color::yellow), "Password: {}\n", password);
        fmt::print(fmt::fg(fmt::color::yellow), "Battle Times: {}\n", battleTimes);
        fmt::print(fmt::fg(fmt::color::yellow), "Winner: {}\n", winner);
        fmt::print(fmt::fg(fmt::color::yellow), "Win Rate: {:.2f}%\n", getRate() * 100);

        fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "Medals:\n");
        for (const auto& medal : medals) {
            fmt::print(fmt::fg(fmt::color::green), "  - {}\n", medal);
        }

        fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "Sprites:\n");
        for (const auto& sprite : sprites) {
            sprite.showInfo();  // Assuming sprite.showInfo() also uses fmt for output
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
            nlohmann::json jsonSprite = JsonSprite(sprite);
            j["sprites"].push_back(jsonSprite);
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
        for (const auto& sprite : j["sprites"]) {
            sprites.push_back(*sprite.get<JsonSprite>().toSprite());
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

    void showSpriteInfo() const{
        for (const auto& sprite : sprites) {
            JsonSprite(sprite).showInfo();
        }
    }

    std::shared_ptr<JsonSprite> getSprite(const std::string& spriteName) const {
        for (const auto& sprite : sprites) {
            if (sprite.getSpriteName() == spriteName) {
                return std::make_shared<JsonSprite>(sprite);
            }
        }
        return nullptr;
    }

    std::vector<JsonSprite> getRandomSprites(int size = 3){
        std::vector<JsonSprite> randomSprites;
        for (const auto& sprite : sprites) {
            randomSprites.push_back(JsonSprite(sprite));
        }
        // 将精灵随机排序
        std::default_random_engine rng(unsigned(std::time(0)));
        std::shuffle(randomSprites.begin(), randomSprites.end(), rng);
        randomSprites.resize(size < randomSprites.size() ? size : randomSprites.size());
        return randomSprites;
    }

    std::vector<std::string> getRandomSpriteNames(int size = 3){
        std::vector<std::string> randomSpriteNames;
        for (const auto& sprite : sprites) {
            randomSpriteNames.push_back(sprite.getSpriteName());
        }
        // 将精灵名字随机排序
        std::default_random_engine rng(unsigned(std::time(0)));
        std::shuffle(randomSpriteNames.begin(), randomSpriteNames.end(), rng);
        randomSpriteNames.resize(size < randomSpriteNames.size() ? size : randomSpriteNames.size());
        return randomSpriteNames;
    }

    bool isMedalExist(const std::string& medal){
        for(const auto& m : medals){
            if(m == medal){
                return true;
            }
        }
        return false;
    }
};
