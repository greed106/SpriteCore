#include "SpriteClient/Context.hpp"
#include "SpriteClient/State.hpp"

Context& Context::getInstance(){
    static Context instance;
    return instance;
}

void Context::setState(std::shared_ptr<State> state){
    this->currentState = state;
}

void Context::request(const std::string& command){
    if(currentState){
        currentState->handle(*this, command);
    }else{
        logger->error("No state set");
    }
}

Context::Context(){
    logger = spdlog::stdout_color_mt("Context");
    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");
    initWebSocket();
}

void Context::initWebSocket() {
    wsClient.onopen = [this](){
        this->logger->info("WebSocket opened");
        std::unique_lock<std::mutex> lock(mtx);
        isConnected = true;
        cv.notify_all(); // 通知等待的线程连接已建立
    };
    wsClient.onmessage = [this](const std::string& message) {
        auto j = nlohmann::json::parse(message);
        this->messageQueue.enqueue(j.get<BattleResult>());
        this->logger->info("WebSocket closed");
        this->closeWebSocket();
    };
    wsClient.onclose = [this](){
//        this->logger->info("WebSocket closed");
        this->isConnected = false;
    };
}

void Context::openWebSocket() {
    wsClient.open(wsUrl.c_str());
}
