#pragma once

#include <iostream>

class Buff {
private:
    // Buff持续回合
    int duration;
    // 攻击加成百分比
    double attackRate;
    // 防御加成百分比
    double defenseRate;
    // 速度加成百分比
    double speedRate;
    // 攻击增量
    int attackIncrease;
    // 防御增量
    int defenseIncrease;
    // 速度增量
    int speedIncrease;
    // 是否是对自己的Buff
    bool forSelf;
public:
    Buff(int duration = 1, double attackRate = 0, double defenseRate = 0,
        double speedRate = 0, int attackIncrease = 0, int defenseIncrease = 0,
        int speedIncrease = 0, bool forSelf = true) :
        duration(duration), attackRate(attackRate), defenseRate(defenseRate),
        speedRate(speedRate), attackIncrease(attackIncrease), defenseIncrease(defenseIncrease),
        speedIncrease(speedIncrease), forSelf(forSelf)
    {
    }
    void showInfo() const{
        // 输出Buff的信息
        std::cout << "Buff Info:" << std::endl;
        std::cout << "Duration: " << duration << std::endl;
        std::cout << "Attack Rate: " << attackRate << std::endl;
        std::cout << "Defense Rate: " << defenseRate << std::endl;
        std::cout << "Speed Rate: " << speedRate << std::endl;
        std::cout << "Attack Increase: " << attackIncrease << std::endl;
        std::cout << "Defense Increase: " << defenseIncrease << std::endl;
        std::cout << "Speed Increase: " << speedIncrease << std::endl;
        std::cout << "For Self: " << (forSelf ? "Yes" : "No") << std::endl;
    }
    bool isForSelf() const{
        return forSelf;
    }
    int getDuration() const{
        return duration;
    }
    double getAttackRate() const{
        return attackRate;
    }
    double getDefenseRate() const{
        return defenseRate;
    }
    double getSpeedRate() const{
        return speedRate;
    }
    int getAttackIncrease() const{
        return attackIncrease;
    }
    int getDefenseIncrease() const{
        return defenseIncrease;
    }
    int getSpeedIncrease() const{
        return speedIncrease;
    }
    bool isExpired() const{
        return duration <= 0;
    }
    void decreaseDuration(){
        duration--;
    }
};