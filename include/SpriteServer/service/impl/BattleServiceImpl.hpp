#pragma once

#include "SpriteServer/service/BattleService.hpp"
#include "SpriteBattle/SpriteBattle.hpp"
#include "SpriteBattle/BattleQueue.hpp"

class BattleServiceImpl : public BattleService {
private:
    BattleQueue::RequestQueue& reqQueue = BattleQueue::RequestQueue::getInstance();
    BattleQueue::ResultQueue& resultQueue = BattleQueue::ResultQueue::getInstance();
    std::atomic<bool> isRunning = true;
    std::thread battleThread;

    BattleServiceImpl(){
        battleThread = std::thread([this]{
            this->battleLoop();
        });
    }

    ~BattleServiceImpl(){
        isRunning = false;
        reqQueue.shutdownQueue();
        if(battleThread.joinable()){
            battleThread.join();
        }
    }
public:
    static BattleServiceImpl& getInstance() {
        static BattleServiceImpl instance;
        return instance;
    }

    BattleResult battleWithComputer(const BattleRequest &request, const BattleNode &computer) override {
        auto username = request.getUsername();
        // TODO: 存在数据一致性问题，如果用户请求中的精灵参数和数据库中的不一致，会导致战斗结果不准确
        auto challenger = *(request.getChallenger().toSprite());
        return SpriteBattle::battle({username, challenger}, computer);
    }

    void battleLoop(){
        while(isRunning){
            auto req1 = reqQueue.dequeue();
            if(!req1.has_value()){
                continue;
            }
            auto req2 = reqQueue.dequeue();
            if(!req2.has_value()){
                reqQueue.enqueue(req1.value());
                continue;
            }
            // TODO: 存在数据一致性问题，如果用户请求中的精灵参数和数据库中的不一致，会导致战斗结果不准确
            auto challenger = req1.value().getChallenger().toSprite();
            auto challengerName = req1.value().getUsername();
            auto opponent = req2.value().getChallenger().toSprite();
            auto opponentName = req2.value().getUsername();

            BattleNode challengerNode(challengerName, *challenger);
            BattleNode opponentNode(opponentName, *opponent);

            auto result = SpriteBattle::battle(challengerNode, opponentNode);

            resultQueue.enqueue({req1.value(), result});
            resultQueue.enqueue({req2.value(), result});
        }
    }
};