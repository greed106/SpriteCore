#pragma once

#include <iostream>

class Level {
private:
    static const int maxLevel = 15;
    int level;
    int exp;
public:
    Level(int level, int exp) : level(level), exp(exp)
    {
        while(canLevelUp()) {
            this->exp -= getNeededExp(this->level + 1);
            this->level++;
        }
    }

    void showInfo() const{
        std::cout << "Level: " << level << std::endl;
    }

    static int getNeededExp(const int level) {
        return level * level;
    }

    void levelUp() {
        while(canLevelUp()) {
            exp -= getNeededExp(level + 1);
            level++;
        }
    }
    
    bool canLevelUp() const {
        if(level >= maxLevel)
            return false;
        return exp >= getNeededExp(level + 1);
    }

    void addExp(int exp) {
        this->exp += exp;
        levelUp();
    }

    int getLevel() const {
        return level;
    }

    int getExp() const {
        return exp;
    }

    void setLevel(int level) {
        this->level = level;
    }

    void setExp(int exp) {
        this->exp = exp;
    }
};