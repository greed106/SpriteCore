#pragma once

#include "Sprite.hpp"
#include "Stirps.hpp"
#include <functional>
#include <typeindex>
#include <map>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>

// 静态初始化类，用于初始化随机数种子
class RandomInitializer {
public:
    RandomInitializer() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }
};

class Factory {
public:
    using Creator = std::function<Stirps*(int)>;

    template <typename T>
    static void registerCreator() {
        std::string typeName = typeid(T).name();
        getCreators()[typeName] = [](int level) {
            return new T(level);
        };
    }

    static std::shared_ptr<Sprite> create(const std::string& typeName, const std::string& spriteName = "", int level = 1, int exp = 0) {
        auto it = getCreators().find(typeName);
        if (it == getCreators().end()) {
            return nullptr;
        }
        Stirps* stirps = it->second(level);
        return std::make_shared<Sprite>(stirps, spriteName, level, exp);
    }

    static std::shared_ptr<Sprite> createRandomSprite(const std::string& spriteName = "", int level = 1, int exp = 0) {
        static RandomInitializer initializer; // 确保在第一次调用时初始化随机数种子
        const auto& creators = getCreators();
        if (creators.empty()) {
            return nullptr;
        }
        int randIndex = std::rand() % creators.size();
        auto it = std::begin(creators);
        std::advance(it, randIndex);
        Stirps* stirps = it->second(level);
        return std::make_shared<Sprite>(stirps, spriteName, level, exp); // 直接传递stirps指针
    }

private:
    static std::map<std::string, Creator>& getCreators() {
        static std::map<std::string, Creator> creators;
        return creators;
    }
};

template<typename T>
class Register {
public:
    Register() {
        Factory::registerCreator<T>();
    }
};
