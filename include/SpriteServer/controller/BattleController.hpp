#pragma once

#include <hv/WebSocketServer.h>
#include "SpriteServer/service/ServiceManager.hpp"
#include "SpriteServer/utils/MessageQueue.hpp"
#include "SpriteBattle/BattleResult.hpp"
#include "SpriteBattle/BattleQueue.hpp"

class BattleController{
private:
    std::map<std::string, WebSocketChannelPtr> clients;
    BattleService* battleService;
    UserService* userService;
    SpriteService* spriteService;
    std::atomic<bool> isRunning = true;
    BattleQueue::RequestQueue& reqQueue;
    BattleQueue::ResultQueue& resultQueue;
    std::thread resultThread;

    static void onopen(const WebSocketChannelPtr& channel, const HttpRequestPtr& req){
        BattleController::getInstance().logger->info("WebSocket connection opened");
    }
    static void onmessage(const WebSocketChannelPtr& channel, const std::string& msg){
        BattleController::getInstance().handleBattleRequest(msg, channel);
    }
    static void onclose(const WebSocketChannelPtr& channel){
        BattleController::getInstance().logger->info("WebSocket connection closed");
    }
    static std::string getComputerName(){
        return "computer";
    }
    BattleController() :
        reqQueue(BattleQueue::RequestQueue::getInstance()),
        resultQueue(BattleQueue::ResultQueue::getInstance()),
        battleService(ServiceManager::getInstance().getBattleService()),
        userService(ServiceManager::getInstance().getUserService()),
        spriteService(ServiceManager::getInstance().getSpriteService())
    {
        resultThread = std::thread([this](){
            handleBattleResult();
        });

        logger = spdlog::stdout_color_mt("BattleController");
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");
    }
    ~BattleController(){
        isRunning = false;
        resultQueue.shutdownQueue();

        if(resultThread.joinable()){
            resultThread.join();
        }
    }
public:
    std::shared_ptr<spdlog::logger> logger;

    // 删除拷贝构造函数和赋值运算符
    BattleController(const BattleController&) = delete;
    BattleController& operator=(const BattleController&) = delete;

    static BattleController& getInstance(){
        static BattleController instance;
        return instance;
    }
    static void registerWebSocketService(hv::WebSocketService& ws){
        ws.onopen = onopen;
        ws.onmessage = onmessage;
        ws.onclose = onclose;
    }
    void handleBattleRequest(const std::string &msg, const WebSocketChannelPtr& channel) {
        auto j = nlohmann::json::parse(msg);
        auto request = j.get<BattleRequest>();

        clients[request.getUsername()] = channel;

        if(!request.getIsComputer()){
            reqQueue.enqueue(request);
        }else{
            auto s = spriteService->getSprite(getComputerName(), request.getOpponentName());
            auto computerNode = BattleNode(getComputerName(), *s);
            auto result = battleService->battleWithComputer(request, computerNode);
            resultQueue.enqueue({request, result});

            JsonSprite prisoner(*s);
            BattleRequest computerRequest(getComputerName(), prisoner, prisoner, true, request.getUsername());
            resultQueue.enqueue({computerRequest, result});
        }
    }
    void handleBattleResult(){
        while(isRunning){
            auto result = resultQueue.dequeue();
            if(!result.has_value()){
                continue;
            }
            auto req = result.value().first;
            auto res = result.value().second;
            // TODO: 同样存在数据一致性问题，数据库中的信息最后会被更新为用户指定的参数
            auto challenger = req.getChallenger().toSprite();
            auto prisonerName = req.getPrisoner().getSpriteName();

            // 根据战斗结果修改对应数据
            if(req.getUsername() == res.getWinner()){
                challenger->addExp(res.getWinnerExp());
                spriteService->updateSprite(req.getUsername(), *challenger);
                userService->addWinner(req.getUsername());
            }else{
                challenger->addExp(res.getLoserExp());
                spriteService->updateSprite(req.getUsername(), *challenger);
                userService->addBattleTimes(req.getUsername());

                auto newName = prisonerName;
                while(spriteService->isSpriteExist(
                        res.getWinner(), newName))
                {
                    newName.append("_COPY");
                }

                auto prisoner = spriteService->getSprite(res.getLoser(), prisonerName);
                prisoner->setSpriteName(newName);
                spriteService->removeSprite(res.getLoser(), prisonerName);
                spriteService->addSprite(res.getWinner(), *prisoner);
            }

            if(req.getUsername() == getComputerName() && req.getIsComputer()){
                continue;
            }

            auto client = clients[req.getUsername()];
            nlohmann::json j = res;
            client->send(j.dump());
            client->close();
            clients.erase(req.getUsername());
        }
    }
};