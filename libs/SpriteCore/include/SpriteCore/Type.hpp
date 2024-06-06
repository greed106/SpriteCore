#pragma once

#include <string>
#include <iostream>

class Type {
protected:
    // 攻击力
    int attack;
    // 防御力
    int defense;
    // 生命值
    int health;
    // 攻击速度
    int attackSpeed;
    // 类型名称
    std::string typeName;
public:
    Type(int attack, int defense, int health, int attackSpeed, std::string typeName);
    int getAttack() const;
    int getDefense() const;
    int getHealth() const;
    int getAttackSpeed() const;
    void setAttack(int attack);
    void setDefense(int defense);
    void setHealth(int health);
    void setAttackSpeed(int attackSpeed);
    std::string getTypeName() const;
    void showInfo() const;
    // 属性增长函数
    virtual void levelUp() = 0;
    virtual ~Type() = default;
    virtual Type* clone() = 0;
};

// 速度型
class SpeedType : public Type {
public:
    SpeedType(int attack, int defense, int health, int attackSpeed);
    void levelUp() override;
    Type* clone() override;
};

// 力量型
class PowerType : public Type {
public:
    PowerType(int attack, int defense, int health, int attackSpeed);
    void levelUp() override;
    Type* clone() override;
};

// 防御型
class DefenseType : public Type {
public:
    DefenseType(int attack, int defense, int health, int attackSpeed);
    void levelUp() override;
    Type* clone() override;
};

// 肉盾型
class TankType : public Type {
public:
    TankType(int attack, int defense, int health, int attackSpeed);
    void levelUp() override;
    Type* clone() override;
};