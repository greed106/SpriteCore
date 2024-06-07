#pragma once

#include "Stirps.hpp"
#include "Level.hpp"
#include <nlohmann/json.hpp>
#include <typeindex>

class Sprite {
private:
    std::unique_ptr<Stirps> stirps;
    std::string spriteName;
    Level level;
public:
    Sprite(Stirps* stirps, std::string spriteName, int level = 1, int exp = 0);
    void showInfo() const;
    void addExp(int exp);
    // 拷贝构造函数
    Sprite(const Sprite &sprite);
    // 拷贝赋值运算符
    Sprite& operator=(const Sprite &sprite);
    // 移动构造函数
    Sprite(Sprite &&sprite) noexcept;
    // 移动赋值运算符
    Sprite& operator=(Sprite &&sprite) noexcept;
    std::string getSpriteName() const;
    static nlohmann::json toJson(const Sprite& sprite);
    static std::shared_ptr<Sprite> fromJson(const nlohmann::json& j);
    std::shared_ptr<Type> getType() const;
    Buff getBuff() const;
    Level getLevel() const;
    void setSpriteName(const std::string& newName);
};