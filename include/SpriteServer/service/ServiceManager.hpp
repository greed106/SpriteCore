#pragma once

#include "UserService.hpp"
#include "SpriteService.hpp"
#include "impl/UserServiceImpl.hpp"
#include "impl/SpriteServiceImpl.hpp"
#include "impl/BattleServiceImpl.hpp"

class ServiceManager {
public:
    static ServiceManager& getInstance() {
        static ServiceManager instance;
        return instance;
    }

    UserService* getUserService() {
        return &UserServiceImpl::getInstance();
    }

    SpriteService* getSpriteService() {
        return &SpriteServiceImpl::getInstance();
    }

    BattleService* getBattleService() {
        return &BattleServiceImpl::getInstance();
    }

    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;
private:
    ServiceManager() = default;
    ~ServiceManager() = default;
};
