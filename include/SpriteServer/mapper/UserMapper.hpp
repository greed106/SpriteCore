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
                   "rate DOUBLE,"
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
        double rate = 0;

        Poco::Data::Statement select(session);
        select << "SELECT password, rate FROM user WHERE name = ?",
                Poco::Data::Keywords::into(password),
                Poco::Data::Keywords::into(rate),
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::now;

        if (!password.empty()) {
            auto user = std::make_shared<User>(name, password, rate);
            auto medals = getMedalsByName(name);
            for (const auto& medal : medals) {
                user->addMedal(medal);
            }
            return user;
        }
        return nullptr;
    }

    void addUser(const User& user) {
        Poco::Data::Session session(pool->get());
        std::string name = user.getName();
        std::string password = user.getPassword();
        double rate = user.getRate();

        Poco::Data::Statement insert(session);
        insert << "INSERT INTO user (name, password, rate) VALUES (?, ?, ?)",
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::useRef(password),
                Poco::Data::Keywords::use(rate),
                Poco::Data::Keywords::now;
    }

    void updateUser(const User& user) {
        Poco::Data::Session session(pool->get());
        std::string name = user.getName();
        std::string password = user.getPassword();
        double rate = user.getRate();

        Poco::Data::Statement update(session);
        update << "UPDATE user SET password = ?, rate = ? WHERE name = ?",
                Poco::Data::Keywords::useRef(password),
                Poco::Data::Keywords::use(rate),
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
};
