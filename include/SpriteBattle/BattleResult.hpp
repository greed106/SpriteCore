#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fmt/core.h>
#include <fmt/color.h>

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
        fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "Battle Information:\n");
        fmt::print(fmt::fg(fmt::color::yellow), "Winner: {}\n", winner);
        fmt::print(fmt::fg(fmt::color::yellow), "Winner Exp: {}\n", winnerExp);
        fmt::print(fmt::fg(fmt::color::red), "Loser: {}\n", loser);
        fmt::print(fmt::fg(fmt::color::red), "Loser Exp: {}\n", loserExp);

        fmt::print(fmt::fg(fmt::color::green) | fmt::emphasis::bold, "Logs:\n");
        for (const auto& log : logs) {
            fmt::print(fmt::fg(fmt::color::white), "{}\n", log);
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