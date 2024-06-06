#pragma once

#include "Mapper.hpp"
#include "SpriteCore/Sprite.hpp"
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <iostream>

class SpriteMapper : public Mapper {
private:
    SpriteMapper() : Mapper() {
        Poco::Data::Session session(pool->get());
        session << "CREATE TABLE IF NOT EXISTS user_sprite ("
                   "name VARCHAR(100),"
                   "sprite TEXT"
                   ")", Poco::Data::Keywords::now;
    }

public:
    static SpriteMapper& getInstance() {
        static SpriteMapper instance;
        return instance;
    }

    std::vector<Sprite> getSpritesByName(const std::string& name) {
        Poco::Data::Session session(pool->get());
        std::vector<Sprite> sprites;
        std::vector<std::string> spriteStrs;

        try {
            Poco::Data::Statement select(session);
            select << "SELECT sprite FROM user_sprite WHERE name = ?",
                    Poco::Data::Keywords::into(spriteStrs),
                    Poco::Data::Keywords::useRef(name),
                    Poco::Data::Keywords::now;

            for (const auto& spriteStr : spriteStrs) {
                nlohmann::json j = nlohmann::json::parse(spriteStr);
                sprites.push_back(*Sprite::fromJson(j));
            }
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving sprites: " << e.what() << std::endl;
        }
        return sprites;
    }

    void addSprite(const std::string& name, const Sprite& sprite) {
        Poco::Data::Session session(pool->get());
        nlohmann::json j = Sprite::toJson(sprite);
        std::string spriteStr = j.dump();
        Poco::Data::Statement insert(session);
        insert << "INSERT INTO user_sprite (name, sprite) VALUES (?, ?)",
                Poco::Data::Keywords::useRef(name),
                Poco::Data::Keywords::use(spriteStr),
                Poco::Data::Keywords::now;
    }
};
