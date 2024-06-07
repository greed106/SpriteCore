#pragma once

#include "SpriteServer/pojo/DTO/BattleRequest.hpp"
#include "SpriteBattle/BattleResult.hpp"
#include "SpriteBattle/SpriteBattle.hpp"

class BattleService {
protected:
    BattleService() = default;
    virtual ~BattleService() = default;
public:
    BattleService(const BattleService&) = delete;
    BattleService& operator=(const BattleService&) = delete;
    virtual BattleResult battleWithComputer(const BattleRequest &request, const BattleNode &computer) = 0;

};