#pragma once

#include "Stirps.hpp"
#include "Level.hpp"

class Sprite {
private:
    std::unique_ptr<Stirps> stirps;
    std::string spriteName;
    Level level;
public:
    Sprite(Stirps* stirps, std::string spriteName, int level = 1, int exp = 0);
    void showInfo() const;
    int doAttack() const;
    void enhanceTemporarily();
    void addExp(int exp);
    void hurt(int damage);
    Sprite(const Sprite &sprite);
};