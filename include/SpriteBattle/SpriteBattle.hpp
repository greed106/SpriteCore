#pragma once

#include "SpriteCore/Sprite.hpp"
#include "BattleNode.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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