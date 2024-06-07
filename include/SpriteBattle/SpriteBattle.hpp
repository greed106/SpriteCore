#pragma once

#include "SpriteCore/Sprite.hpp"
#include "SpriteBattle/BattleResult.hpp"
#include <queue>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


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

class SpriteBattle {
public:
    static BattleResult battle(BattleNode user1, BattleNode user2){
        std::priority_queue<BattleNode, std::vector<BattleNode>, Compare> queue;
//        auto logger = spdlog::stdout_color_mt("battle");
//        logger->set_pattern("[%n] [%^%l%$] %v");
        user1.clear();
        user2.clear();
        user1.updatePriority();
        user2.updatePriority();

        queue.push(user1);
        queue.push(user2);

        int round = 0;
        std::vector<std::string> logs;

        while(true){
            round++;
            auto attacker = queue.top();
            queue.pop();
            auto defender = queue.top();
            queue.pop();
            Buff buff = attacker.getBuff();
            std::string message = "Round " + std::to_string(round) + ": " + attacker.getUsername() + " attack " + defender.getUsername();
//            logger->info(message);
            logs.push_back(message);

            if(buff.isForSelf()){
                attacker.addBuff(buff);
            }else{
                defender.addBuff(buff);
            }
            message = "attack: " + std::to_string(attacker.getAttack()) + ", defense: " + std::to_string(defender.getDefense());
//            logger->info(message);
            int damage = attacker.getAttack() - defender.getDefense();
            if(damage < 0 || RandomUtil::random() < 0.05){
                damage = 0;
                message = "Miss!";
//                logger->info(message);
                logs.push_back(message);
            }
            defender.hurt(damage);

            message = "attacker health: " + std::to_string(attacker.getHealth()) + ", defender health: " + std::to_string(defender.getHealth());
//            logger->info(message);
            logs.push_back(message);

            if(defender.isDead()){
                std::string winner = attacker.getUsername();
                int winnerExp = getWinnerExp(attacker.getLevel(), defender.getLevel());
                int loserExp = getLoserExp(attacker.getLevel(), defender.getLevel());
                std::string loser = defender.getUsername();
                auto result = BattleResult(winner, loser, winnerExp, loserExp);
                result.setLogs(logs);
                return result;
            }
            defender.updatePriority();

            attacker.expireBuff();
            defender.expireBuff();
            queue.push(defender);
            queue.push(attacker);
        }
    }
private:
    struct Compare{
        bool operator()(const BattleNode& a, const BattleNode& b){
            return a.getPriority() < b.getPriority();
        }
    };
    static int getWinnerExp(int winnerLevel, int loserLevel){
        int delta = loserLevel - winnerLevel;
        return std::round(std::exp(delta) / 10) + 10;
    }
    static int getLoserExp(int winnerLevel, int loserLevel){
        int delta = winnerLevel - loserLevel;
        return std::round(std::exp(delta)) + 5;
    }

};