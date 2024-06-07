#pragma once

#include <string>
#include <iostream>
#include <vector>

class BattleResult {
private:
    std::string winner;
    std::string loser;
    int winnerExp;
    int loserExp;
    std::vector<std::string> logs;
public:
    BattleResult(std::string winner, std::string loser, int winnerExp, int loserExp) :
        winner(winner), loser(loser), winnerExp(winnerExp), loserExp(loserExp)
    {
    }
    BattleResult() = default;
    void showInfo() const {
        std::cout << "Winner: " << winner << std::endl;
        std::cout << "Winner Exp: " << winnerExp << std::endl;
        std::cout << "Loser: " << loser << std::endl;
        std::cout << "Loser Exp: " << loserExp << std::endl;
        std::cout << "Logs: " << std::endl;
        for(const auto& log : logs)
        {
            std::cout << log << std::endl;
        }
    }
    std::string getWinner() const {
        return winner;
    }
    std::string getLoser() const {
        return loser;
    }
    int getWinnerExp() const {
        return winnerExp;
    }
    int getLoserExp() const {
        return loserExp;
    }
    void addLog(const std::string& log) {
        logs.push_back(log);
    }
    std::vector<std::string> getLogs() const {
        return logs;
    }
    void setLogs(const std::vector<std::string> logs) {
        this->logs = logs;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BattleResult, winner, loser, winnerExp, loserExp, logs)
};