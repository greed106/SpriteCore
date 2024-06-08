#pragma once

#include "Mapper.hpp"
#include "SpriteServer/pojo/User.hpp"
#include <vector>
#include <memory>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/MySQL/Connector.h>
#include <iostream>

class UserMapper : public Mapper {
private:
    UserMapper() : Mapper() {
        Poco::Data::Session session(pool->get());
        session << "CREATE TABLE IF NOT EXISTS user ("
                   "name VARCHAR(100) PRIMARY KEY,"
                   "password VARCHAR(100),"
                   "winner INT,"
                   "battle_times INT,"
                   "online BOOL DEFAULT FALSE"
                   ")", Poco::Data::Keywords::now;

        session << "CREATE TABLE IF NOT EXISTS user_medal ("
                   "name VARCHAR(100),"
                   "medal VARCHAR(100)"
                   ")", Poco::Data::Keywords::now;
    }

public:
    static UserMapper& getInstance() {
        static UserMapper instance;
        return instance;
    }

    std::shared_ptr<User> getUserByName(const std::string& name) {
        Poco::Data::Session session(pool->get());
        std::string password;
        int winner = 0;
        int battleTimes = 0;

        Poco::Data::Statement select(session);
        select << "SELECT password, winner, battle_times FROM user WHERE name = ?",
                Poco::Data::Keywords::into(password),
                Poco::Data::Keywords::into(winner),
                Poco::Data::Keywords::into(battleTimes),
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::now;

        if (password.empty()) {
            return nullptr;
        }
        return std::make_shared<User>(name, password, winner, battleTimes);
    }

    void addUser(const User& user) {
        Poco::Data::Session session(pool->get());
        std::string name = user.getName();
        std::string password = user.getPassword();
        int winner = user.getWinner();
        int battleTimes = user.getBattleTimes();

        Poco::Data::Statement insert(session);
        insert << "INSERT INTO user (name, password, winner, battle_times) VALUES (?, ?, ?, ?)",
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::useRef(password),
                Poco::Data::Keywords::use(winner),
                Poco::Data::Keywords::use(battleTimes),
                Poco::Data::Keywords::now;
    }

    void updateUser(const User& user) {
        Poco::Data::Session session(pool->get());
        std::string name = user.getName();
        std::string password = user.getPassword();
        int winner = user.getWinner();
        int battleTimes = user.getBattleTimes();

        Poco::Data::Statement update(session);
        update << "UPDATE user SET password = ?, winner = ?, battle_times = ? WHERE name = ?",
                Poco::Data::Keywords::useRef(password),
                Poco::Data::Keywords::use(winner),
                Poco::Data::Keywords::use(battleTimes),
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::now;
    }

    std::vector<std::string> getMedalsByName(const std::string& name) {
        Poco::Data::Session session(pool->get());
        std::vector<std::string> medals;

        try {
            Poco::Data::Statement select(session);
            select << "SELECT medal FROM user_medal WHERE name = ?",
                    Poco::Data::Keywords::into(medals),
                    Poco::Data::Keywords::useRef(name),
                    Poco::Data::Keywords::now;
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving medals: " << e.what() << std::endl;
        }
        return medals;
    }

    void addMedal(const std::string& name, const std::string& medal) {
        Poco::Data::Session session(pool->get());

        Poco::Data::Statement insert(session);
        insert << "INSERT INTO user_medal (name, medal) VALUES (?, ?)",
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::useRef(medal),
                Poco::Data::Keywords::now;
    }

    void setOnline(const std::string& name, bool online) {
        Poco::Data::Session session(pool->get());

        Poco::Data::Statement update(session);
        update << "UPDATE user SET online = ? WHERE name = ?",
                Poco::Data::Keywords::use(online),
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::now;
    }

    std::vector<std::shared_ptr<User>> getOnlineUsers() {
        Poco::Data::Session session(pool->get());
        std::vector<std::shared_ptr<User>> users;

        try {
            Poco::Data::Statement select(session);
            select << "SELECT name, password, winner, battle_times FROM user WHERE online = TRUE",
                    Poco::Data::Keywords::now;

            Poco::Data::RecordSet rs(select);
            bool more = rs.moveFirst();

            while (more) {
                std::string name = rs[0].convert<std::string>();
                std::string password = rs[1].convert<std::string>();
                int winner = rs[2].convert<int>();
                int battleTimes = rs[3].convert<int>();

                auto user = std::make_shared<User>(name, password, winner, battleTimes);
                users.push_back(user);

                more = rs.moveNext();
            }
        } catch (const Poco::Exception& e) {
            std::cerr << "Error retrieving online users: " << e.displayText() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving online users: " << e.what() << std::endl;
        }

        return users;
    }

    std::vector<std::shared_ptr<User>> getAllUsers() {
        Poco::Data::Session session(pool->get());
        std::vector<std::shared_ptr<User>> users;

        try {
            Poco::Data::Statement select(session);
            select << "SELECT name, password, winner, battle_times FROM user",
                    Poco::Data::Keywords::now;

            Poco::Data::RecordSet rs(select);
            bool more = rs.moveFirst();

            while (more) {
                std::string name = rs[0].convert<std::string>();
                std::string password = rs[1].convert<std::string>();
                int winner = rs[2].convert<int>();
                int battleTimes = rs[3].convert<int>();

                auto user = std::make_shared<User>(name, password, winner, battleTimes);
                users.push_back(user);

                more = rs.moveNext();
            }
        } catch (const Poco::Exception& e) {
            std::cerr << "Error retrieving all users: " << e.displayText() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving all users: " << e.what() << std::endl;
        }

        return users;
    }
};
