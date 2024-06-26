#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include "SpriteServer/utils/MessageQueue.hpp"
#include "SpriteBattle/BattleResult.hpp"

class State;

class Context {
public:
    static Context& getInstance();
    void setState(std::shared_ptr<State> state);
    void request(const std::string& command);
    std::string getBaseUrl() const { return baseUrl; }
    std::string getUsername() const { return username; }
    void setUsername(const std::string& username) { this->username = username; }
    void setExit(bool isExit) { this->isExit = isExit; }
    bool getIsExit() const { return isExit; }
    void openWebSocket();
    void sendMessage(const std::string& message) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return isConnected; }); // 等待连接建立
        wsClient.send(message);
    }
    BattleResult getResult(){
        while(true){
            auto opt = messageQueue.dequeue();
            if(opt.has_value()){
                return opt.value();
            }
        }
    }
    void closeWebSocket(){
        if(isConnected){
            wsClient.close();
        }
    }
    // 删除拷贝构造函数和赋值运算符
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
private:
    std::shared_ptr<State> currentState;
    std::shared_ptr<spdlog::logger> logger;
    std::string baseUrl = "http://localhost:8080";
    std::string wsUrl = "ws://127.0.0.1:8080";
    bool isExit = true;
    std::string username;
    hv::WebSocketClient wsClient;
    MessageQueue<BattleResult> messageQueue;
    bool isConnected = false;
    std::mutex mtx;
    std::condition_variable cv;

    Context();
    void initWebSocket();
};