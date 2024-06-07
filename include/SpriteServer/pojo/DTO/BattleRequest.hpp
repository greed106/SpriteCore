#pragma once

#include "SpriteServer/pojo/JsonSprite.hpp"

class BattleRequest {
private:
    std::string username;
    JsonSprite challenger;
    JsonSprite prisoner;
    bool isComputer;
    std::string opponentName;
public:
    BattleRequest(std::string username, JsonSprite challenger, JsonSprite prisoner, bool isComputer, std::string opponentName)
        : username(username), challenger(challenger), prisoner(prisoner), isComputer(isComputer), opponentName(opponentName) {}
    BattleRequest(){

    }
    std::string getUsername() const {
        return username;
    }
    JsonSprite getChallenger() const {
        return challenger;
    }
    JsonSprite getPrisoner() const {
        return prisoner;
    }
    bool getIsComputer() const {
        return isComputer;
    }
    std::string getOpponentName() const {
        return opponentName;
    }
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BattleRequest, username, challenger, prisoner, isComputer, opponentName)
};