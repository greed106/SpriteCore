#include "SpriteCore/Sprite.hpp"

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

int Sprite::doAttack() const
{
    std::cout << spriteName << " is attacking!" << std::endl;
    return stirps->doAttack();
}

void Sprite::enhanceTemporarily()
{
    std::cout << spriteName << " is enhancing!" << std::endl;
    stirps->enhance();
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

void Sprite::hurt(int damage)
{
    if(stirps->hurt(damage)) {
        std::cout << spriteName << " is fainted!" << std::endl;
    } else {
        std::cout << spriteName << " is hurt!" << std::endl;
    }
}

Sprite::Sprite(const Sprite &sprite) :
    stirps(sprite.stirps->clone()), spriteName(sprite.spriteName), level(sprite.level)
{
}