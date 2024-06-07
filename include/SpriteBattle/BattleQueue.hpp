#pragma once

#include "SpriteServer/utils/MessageQueue.hpp"
#include "SpriteServer/pojo/DTO/BattleRequest.hpp"
#include "BattleResult.hpp"

namespace BattleQueue{

class RequestQueue{
private:
    MessageQueue<BattleRequest> reqQueue;
    RequestQueue() = default;
    ~RequestQueue(){
        reqQueue.shutdownQueue();
    }
public:
    void enqueue(const BattleRequest& request){
        reqQueue.enqueue(request);
    }
    std::optional<BattleRequest> dequeue(){
        return reqQueue.dequeue();
    }
    static RequestQueue& getInstance(){
        static RequestQueue instance;
        return instance;
    }
    void shutdownQueue(){
        reqQueue.shutdownQueue();
    }
};

class ResultQueue{
private:
    MessageQueue<std::pair<BattleRequest, BattleResult>> resultQueue;
    ResultQueue() = default;
    ~ResultQueue(){
        resultQueue.shutdownQueue();
    }
public:
    void enqueue(const std::pair<BattleRequest, BattleResult>& result){
        resultQueue.enqueue(result);
    }
    std::optional<std::pair<BattleRequest, BattleResult>> dequeue(){
        return resultQueue.dequeue();
    }
    static ResultQueue& getInstance(){
        static ResultQueue instance;
        return instance;
    }
    void shutdownQueue(){
        resultQueue.shutdownQueue();
    }
};

}