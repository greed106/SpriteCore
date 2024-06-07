#pragma once

#include "Mapper.hpp"
#include "SpriteCore/Sprite.hpp"
#include "SpriteServer/pojo/JsonSprite.hpp"
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
                   "username VARCHAR(100),"
                   "sprite_name VARCHAR(100),"
                   "sprite TEXT"
                   ")", Poco::Data::Keywords::now;
    }

public:
    static SpriteMapper& getInstance() {
        static SpriteMapper instance;
        return instance;
    }

    std::vector<Sprite> getSpritesByName(const std::string& username) {
        Poco::Data::Session session(pool->get());
        std::vector<Sprite> sprites;
        std::vector<std::string> spriteStrs;

        try {
            Poco::Data::Statement select(session);
            select << "SELECT sprite FROM user_sprite WHERE username = ?",
                    Poco::Data::Keywords::into(spriteStrs),
                    Poco::Data::Keywords::useRef(username),
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

    void addSprite(const std::string& username, const Sprite& sprite) {
        Poco::Data::Session session(pool->get());
        nlohmann::json j = Sprite::toJson(sprite);

        std::string spriteStr = j.dump();
        std::string spriteName = sprite.getSpriteName();
        try {
            Poco::Data::Statement insert(session);
            insert << "INSERT INTO user_sprite (username, sprite_name, sprite) VALUES (?, ?, ?)",
                    Poco::Data::Keywords::useRef(username),
                    Poco::Data::Keywords::use(spriteName),
                    Poco::Data::Keywords::use(spriteStr),
                    Poco::Data::Keywords::now;
        } catch (const std::exception& e) {
            std::cerr << "Error adding sprite: " << e.what() << std::endl;
        }
    }

    void removeSprite(const std::string& username, const std::string& spriteName) {
        Poco::Data::Session session(pool->get());
        try {
            Poco::Data::Statement remove(session);
            remove << "DELETE FROM user_sprite WHERE username = ? AND sprite_name = ?",
                    Poco::Data::Keywords::useRef(username),
                    Poco::Data::Keywords::useRef(spriteName),
                    Poco::Data::Keywords::now;
        } catch (const std::exception& e) {
            std::cerr << "Error removing sprite: " << e.what() << std::endl;
        }
    }

    void updateSprite(const std::string& username, const Sprite& sprite) {
        Poco::Data::Session session(pool->get());
        nlohmann::json j = Sprite::toJson(sprite);
        std::string spriteStr = j.dump();
        std::string spriteName = sprite.getSpriteName();
        try {
            Poco::Data::Statement update(session);
            update << "UPDATE user_sprite SET sprite = ? WHERE username = ? AND sprite_name = ?",
                    Poco::Data::Keywords::use(spriteStr),
                    Poco::Data::Keywords::useRef(username),
                    Poco::Data::Keywords::useRef(spriteName),
                    Poco::Data::Keywords::now;
        } catch (const std::exception& e) {
            std::cerr << "Error updating sprite: " << e.what() << std::endl;
        }
    }

    JsonSprite getSprite(const std::string& username, const std::string& spriteName) {
        Poco::Data::Session session(pool->get());
        std::string spriteStr;
        try {
            Poco::Data::Statement select(session);
            select << "SELECT sprite FROM user_sprite WHERE username = ? AND sprite_name = ?",
                    Poco::Data::Keywords::into(spriteStr),
                    Poco::Data::Keywords::useRef(username),
                    Poco::Data::Keywords::useRef(spriteName),
                    Poco::Data::Keywords::now;
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving sprite: " << e.what() << std::endl;
        }
        nlohmann::json j = nlohmann::json::parse(spriteStr);
        JsonSprite jsonSprite = j.get<JsonSprite>();
        return jsonSprite;
    }
};
