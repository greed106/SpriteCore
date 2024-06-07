#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SpriteCore/Factory.hpp"

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

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(JsonSprite, type, spriteName, level, exp)
};