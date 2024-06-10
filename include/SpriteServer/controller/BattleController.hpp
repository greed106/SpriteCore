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

        spdlog::drop("BattleController");
    }
    std::string getNewPrisonerName(const std::string& username, const std::string& name){
        auto newName = name;
        while(spriteService->isSpriteExist(username, newName)){
            newName.append("_COPY");
        }
        return newName;
    }
    void handleSpriteLevelUp(const BattleRequest& req, const BattleResult& res){
        auto challenger = req.getChallenger().toSprite();
        auto isWinner = req.getUsername() == res.getWinner();
        challenger->addExp(isWinner ? res.getWinnerExp() : res.getLoserExp());

        spriteService->updateSprite(req.getUsername(), *challenger);
    }
    void handleUserRate(const std::string& username, bool isWinner){
        if(isWinner){
            userService->addWinner(username);
        }else{
            userService->addBattleTimes(username);
        }
    }
    void handleSpriteChange(BattleRequest& req, BattleResult& res){
        // TODO: 同样存在数据一致性问题，数据库中的信息最后会被更新为用户指定的参数
        auto challenger = req.getChallenger().toSprite();
        auto prisonerName = req.getPrisoner().getSpriteName();

        // 根据战斗结果修改对应数据
        auto isWinner = req.getUsername() == res.getWinner();
        handleSpriteLevelUp(req, res);
        handleUserRate(req.getUsername(), isWinner);
        if(!isWinner){
            auto newName = getNewPrisonerName(res.getWinner(), prisonerName);
            auto prisoner = spriteService->getSprite(res.getLoser(), prisonerName);
            prisoner->setSpriteName(newName);

            spriteService->removeSprite(res.getLoser(), prisonerName);
            spriteService->addSprite(res.getWinner(), *prisoner);
        }

        if(spriteService->getSprites(req.getUsername()).size() == 0){
            auto sprite = Factory::createRandomSprite("RandomSprite");
            spriteService->addSprite(req.getUsername(), *sprite);
        }
    }
    int getMaxSpriteSize(std::vector<Sprite> sprites){
        int cnt = 0;
        for(const auto& sprite : sprites){
            if(sprite.getLevel().getLevel() == 15)
                cnt++;
        }
        return cnt;
    }
    void handleMedalChange(const std::string& username){
        auto user = userService->getUser(username);
        auto sprites = spriteService->getSprites(user->getName());
        auto size = sprites.size();
        std::string medal = "bronze_number_medal";
        if( size >= 4 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
        medal = "silver_number_medal";
        if( size >= 8 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
        medal = "gold_number_medal";
        if( size >= 12 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
        size = getMaxSpriteSize(sprites);
        medal = "bronze_level_medal";
        if( size >= 1 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
        medal = "silver_level_medal";
        if( size >= 3 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
        medal = "gold_level_medal";
        if( size >= 5 && !user->isMedalExist(medal)){
            userService->addMedal(username, medal);
        }
    }
    BattleRequest getComputerBattleRequest(const BattleRequest& userReq){
        auto s = spriteService->getSprite(getComputerName(), userReq.getOpponentName());
        JsonSprite prisoner(*s);
        BattleRequest computerRequest(getComputerName(), prisoner, prisoner, true, userReq.getUsername());
        return computerRequest;
    }
    void handleBattleRequest(const std::string &msg, const WebSocketChannelPtr& channel) {
        auto j = nlohmann::json::parse(msg);
        auto request = j.get<BattleRequest>();
        logger->info("Handling battle request from {}", request.getUsername());
        clients[request.getUsername()] = channel;

        if(!request.getIsComputer()){
            reqQueue.enqueue(request);
        }else{
            auto s = spriteService->getSprite(getComputerName(), request.getOpponentName());
            auto computerNode = BattleNode(getComputerName(), *s);
            auto result = battleService->battleWithComputer(request, computerNode);
            resultQueue.enqueue({request, result});

            auto computerRequest = getComputerBattleRequest(request);
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
            handleSpriteChange(req, res);
            handleMedalChange(req.getUsername());

            if(req.getUsername() == getComputerName() && req.getIsComputer()){
                continue;
            }

            auto client = clients[req.getUsername()];
            nlohmann::json j = res;
            client->send(j.dump());

            clients.erase(req.getUsername());
        }
    }
    int handleLevelUpBattle(const HttpContextPtr& ctx){
        auto j = nlohmann::json::parse(ctx->body());
        auto req = j.get<BattleRequest>();
        auto res = battleService->battleWithComputer(req, BattleNode(getComputerName(), *spriteService->getSprite(getComputerName(), req.getOpponentName())));
        auto computerReq = getComputerBattleRequest(req);
        auto isWinner = req.getUsername() == res.getWinner();
        handleSpriteLevelUp(req, res);
        handleSpriteLevelUp(computerReq, res);
        handleUserRate(req.getUsername(), isWinner);
        handleUserRate(computerReq.getUsername(), !isWinner);
        handleMedalChange(req.getUsername());

        return ctx->send(nlohmann::json(res).dump());
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
    static void registerRoutes(hv::HttpService& router){
        router.POST("/battle", [](const HttpContextPtr& ctx){
            return BattleController::getInstance().handleLevelUpBattle(ctx);
        });
    }
};