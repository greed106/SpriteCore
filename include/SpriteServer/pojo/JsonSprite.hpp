#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SpriteCore/Factory.hpp"
#include <fmt/core.h>
#include <fmt/color.h>

class JsonSprite {
private:
    std::string type;
    std::string spriteName;
    int level;
    int exp;
public:
    JsonSprite(std::string typeName = "", std::string spriteName = "", int level = 1, int exp = 0) :
            type(typeName), spriteName(spriteName), level(level), exp(exp) {}

    JsonSprite(const Sprite& sprite){
        auto j = Sprite::toJson(sprite);
        *this = j.get<JsonSprite>();
    }

    std::string getType() const {
        return this->type;
    }

    std::string getSpriteName() const {
        return this->spriteName;
    }

    int getLevel() const {
        return this->level;
    }

    int getExp() const {
        return this->exp;
    }

    std::shared_ptr<Sprite> toSprite() const {
        return Factory::create(this->type, this->spriteName, this->level, this->exp);
    }

    void showInfo() const {
        fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "JsonSprite Information:\n");
        fmt::print(fmt::fg(fmt::color::yellow), "Type: {}\n", type);
        fmt::print(fmt::fg(fmt::color::yellow), "Sprite Name: {}\n", spriteName);
        fmt::print(fmt::fg(fmt::color::yellow), "Level: {}\n", level);
        fmt::print(fmt::fg(fmt::color::yellow), "Exp: {}\n", exp);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(JsonSprite, type, spriteName, level, exp)
};