#include "SpriteCore/Sprite.hpp"
#include "SpriteCore/Factory.hpp"

Sprite::Sprite(Stirps *stirps, std::string spriteName, int level, int exp) :
    stirps(stirps), spriteName(spriteName), level(level, exp)
{
}

void Sprite::showInfo() const
{
    std::cout << "Sprite: " << spriteName << std::endl;
    level.showInfo();
    stirps->showInfo();
}

void Sprite::addExp(int exp)
{
    int originalLevel = level.getLevel();
    level.addExp(exp);
    while(originalLevel < level.getLevel())
    {
        stirps->enhance();
        originalLevel++;
    }
}

Sprite::Sprite(const Sprite &sprite) :
    stirps(sprite.stirps->clone()), spriteName(sprite.spriteName), level(sprite.level)
{
}

std::string Sprite::getSpriteName() const {
    return spriteName;
}

Sprite &Sprite::operator=(const Sprite &sprite) {
    if(this == &sprite) {
        return *this;
    }
    stirps.reset(sprite.stirps->clone());
    spriteName = sprite.spriteName;
    level = sprite.level;
    return *this;
}

Sprite::Sprite(Sprite &&sprite) noexcept :
    stirps(std::move(sprite.stirps)), spriteName(std::move(sprite.spriteName)), level(std::move(sprite.level))
{
}

Sprite &Sprite::operator=(Sprite &&sprite) noexcept {
    if(this == &sprite) {
        return *this;
    }
    stirps = std::move(sprite.stirps);
    spriteName = std::move(sprite.spriteName);
    level = std::move(sprite.level);
    return *this;
}

nlohmann::json Sprite::toJson(const Sprite& sprite) {
    nlohmann::json j = {
        {"type", typeid(*(sprite.stirps)).name()},
        {"spriteName", sprite.spriteName},
        {"level", sprite.level.getLevel()},
        {"exp", sprite.level.getExp()},
    };
    return j;
}

std::shared_ptr<Sprite> Sprite::fromJson(const nlohmann::json &j) {
std::string type = j.at("type").get<std::string>();
    std::string spriteName = j.at("spriteName").get<std::string>();
    int level = j.at("level").get<int>();
    int exp = j.at("exp").get<int>();
    return Factory::create(type, spriteName, level, exp);
}

std::shared_ptr<Type> Sprite::getType() const {
    return std::shared_ptr<Type>(stirps->getType());
}

Buff Sprite::getBuff() const {
    return stirps->generateBuff();
}

Level Sprite::getLevel() const {
    return level;
}
