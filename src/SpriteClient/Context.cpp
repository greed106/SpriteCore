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
    wsClient.onmessage = [this](const std::string& message) {
        auto j = nlohmann::json::parse(message);
        this->messageQueue.enqueue(j.get<BattleResult>());
        wsClient.close();
        this->logger->info("WebSocket closed");
    };
    wsClient.onclose = [this](){
        this->logger->info("WebSocket closed");
    };
}

void Context::openWebSocket() {
    wsClient.open(wsUrl.c_str());
}
