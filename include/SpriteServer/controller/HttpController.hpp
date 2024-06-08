#pragma once

#include "SpriteServer/service/ServiceManager.hpp"
#include "SpriteServer/pojo/DTO/LoginRequest.hpp"
#include <hv/HttpServer.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


class HttpController {
public:
    static HttpController& getInstance() {
        static HttpController instance;
        return instance;
    }

    static void registerRoutes(hv::HttpService& router) {
        router.GET("/ping", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handlePing(ctx);
        });
        router.GET("/user", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handleGetUser(ctx);
        });
        router.POST("/login", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handleLogin(ctx);
        });
        router.POST("/register", [](const HttpContextPtr& ctx){
            return HttpController::getInstance().handleRegister(ctx);
        });
        router.GET("/all_user", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handleGetAllUserNames(ctx);
        });
        router.GET("/online_user", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handleGetOnlineUserNames(ctx);
        });
        router.GET("/exit", [](const HttpContextPtr& ctx) {
            return HttpController::getInstance().handleExit(ctx);
        });
    }
    // 删除拷贝构造函数和赋值运算符
    HttpController(const HttpController&) = delete;
    HttpController& operator=(const HttpController&) = delete;
private:
    UserService* userService;
    SpriteService* spriteService;
    std::shared_ptr<spdlog::logger> logger;

    HttpController() {
        userService = ServiceManager::getInstance().getUserService();
        spriteService = ServiceManager::getInstance().getSpriteService();
        logger = spdlog::stdout_color_mt("HttpController");
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v");
    }
    ~HttpController() {
        spdlog::drop("HttpController");
    }

    int handlePing(const HttpContextPtr& ctx) {
        logger->info("Handling ping request");
        return ctx->send("pong");
    }

    int handleGetUser(const HttpContextPtr& ctx) {
        logger->info("Get user: {}", ctx->param("name"));
        auto name = ctx->param("name");
        auto user = userService->getUser(name);
        if (user == nullptr) {
            return ctx->send("User not found");
        }
        auto sprites = spriteService->getSprites(name);
        user->setSprites(sprites);
        auto userStr = User::toJson(*user).dump();
        logger->info("User: {}", userStr);
        return ctx->send(userStr);
    }

    int handleLogin(const HttpContextPtr& ctx) {
        auto j = nlohmann::json::parse(ctx->body());
        auto loginRequest = j.get<LoginRequest>();
        auto username = loginRequest.getUsername();
        auto password = loginRequest.getPassword();
        logger->info("Login request: username={} password={}",
             username, password);

        hv::Json resp;
        auto validLogin = userService->login(username, password);
        resp["valid"] = validLogin;
        if(validLogin){
            userService->setOnline(username, true);
        }
        return ctx->send(resp.dump());
    }

    int handleRegister(const HttpContextPtr& ctx){
        auto registerReq = nlohmann::json::parse(ctx->body()).get<LoginRequest>();
        auto username = registerReq.getUsername();
        auto password = registerReq.getPassword();
        logger->info("Register request: username={} password={}",
             username, password);

        auto isExist = userService->isUserNameExist(username);
        hv::Json resp;
        resp["valid"] = !isExist;
        if(!isExist){
            userService->addUser(username, password);
            for(int i = 1; i <= 3; i++){
                auto name = "RandomSprite_" + std::to_string(i);
                auto sprite = Factory::createRandomSprite(name);
                spriteService->addSprite(username, *sprite);
            }
        }
        return ctx->send(resp.dump());
    }

    int handleGetAllUserNames(const HttpContextPtr& ctx){
        logger->info("Get all usernames");
        auto names = userService->getAllUserNames();
        hv::Json resp;
        resp["names"] = names;
        return ctx->send(resp.dump());
    }

    int handleGetOnlineUserNames(const HttpContextPtr& ctx){
        logger->info("Get online usernames");
        auto names = userService->getOnlineUserNames();
        hv::Json resp;
        resp["names"] = names;
        return ctx->send(resp.dump());
    }

    int handleExit(const HttpContextPtr& ctx){
        auto name = ctx->param("name");
        logger->info("Exit: {}", name);
        userService->setOnline(name, false);
        hv::Json resp;
        resp["valid"] = true;
        return ctx->send(resp.dump());
    }
};