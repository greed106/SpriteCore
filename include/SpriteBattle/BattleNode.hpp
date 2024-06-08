#pragma once

#include "SpriteBattle/BattleResult.hpp"
#include <queue>

class BattleNode{
private:
    std::string username;
    Sprite sprite;
    std::shared_ptr<Type> type;
    long long int priority;
    std::queue<Buff> buffs;
public:
    long long int getPriority() const {
        return this->priority;
    }
    void updatePriority(){
        int baseSpeed = type->getAttackSpeed();
        std::queue<Buff> temp = buffs;
        double rate = 1;
        int increase = 0;
        while(!temp.empty()){
            rate += temp.front().getSpeedRate();
            increase += temp.front().getSpeedIncrease();
            temp.pop();
        }
        this->priority += baseSpeed * rate + increase;
    }
    BattleNode(std::string username, Sprite sprite) : username(username), sprite(sprite){
        this->type = sprite.getType();
        this->priority = 0;
    }
    void addBuff(Buff buff){
        buffs.push(buff);
    }
    int getAttack(){
        int baseAttack = type->getAttack();
        std::queue<Buff> temp = buffs;
        double rate = 1;
        int increase = 0;
        while(!temp.empty()){
            rate += temp.front().getAttackRate();
            increase += temp.front().getAttackIncrease();
            temp.pop();
        }
        return baseAttack * rate + increase;
    }
    void expireBuff(){
        std::queue<Buff> temp;
        while(!buffs.empty()){
            if(buffs.front().getDuration() > 0){
                buffs.front().decreaseDuration();
                temp.push(buffs.front());
            }
            buffs.pop();
        }
        buffs = temp;
    }
    void clear(){
        while(!buffs.empty()){
            buffs.pop();
        }
        priority = 0;
    }
    int getDefense(){
        int baseDefense = type->getDefense();
        std::queue<Buff> temp = buffs;
        double rate = 1;
        int increase = 0;
        while(!temp.empty()){
            rate += temp.front().getDefenseRate();
            increase += temp.front().getDefenseIncrease();
            temp.pop();
        }
        return baseDefense * rate + increase;
    }
    void hurt(int damage){
        int health = type->getHealth();
        health -= damage;
        type->setHealth(health);
    }
    bool isDead(){
        return type->getHealth() <= 0;
    }
    Buff getBuff(){
        return sprite.getBuff();
    }
    std::string getUsername(){
        return username;
    }
    int getLevel(){
        return sprite.getLevel().getLevel();
    }
    int getHealth(){
        return type->getHealth();
    }
};