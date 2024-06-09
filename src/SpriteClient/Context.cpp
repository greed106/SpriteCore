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
    };
    wsClient.onclose = [this](){
        this->logger->info("WebSocket closed");
    };
}

void Context::openWebSocket() {
    // 对baseUrl进行处理，将http替换为ws
    std::string url = baseUrl;
    url.replace(0, 4, "ws");
    wsClient.open(url.c_str());
}
