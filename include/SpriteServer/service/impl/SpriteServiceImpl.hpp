#pragma once

#include "SpriteServer/service/SpriteService.hpp"

class SpriteServiceImpl : public SpriteService {
public:
    static SpriteServiceImpl& getInstance() {
        static SpriteServiceImpl instance;
        return instance;
    }

    std::vector<Sprite> getSprites(std::string name) override {
        return spriteMapper.getSpritesByName(name);
    }

    void addSprite(std::string username, const Sprite& sprite) override {
        spriteMapper.addSprite(username, sprite);
    }

    void removeSprite(std::string username, const std::string &spriteName) override {
        spriteMapper.removeSprite(username, spriteName);
    }

    void updateSprite(std::string username, const Sprite& sprite) override {
        spriteMapper.updateSprite(username, sprite);
    }

    std::shared_ptr<Sprite> getSprite(std::string username, std::string spriteName) override {
        auto jsonSprite = spriteMapper.getSprite(username, spriteName);
        return jsonSprite.toSprite();
    }

    bool isSpriteExist(std::string username, std::string spriteName) override {
        auto sprites = spriteMapper.getSpritesByName(username);
        for (const auto& sprite : sprites) {
            if (sprite.getSpriteName() == spriteName) {
                return true;
            }
        }
        return false;
    }
private:

    SpriteMapper& spriteMapper;

    SpriteServiceImpl() : spriteMapper(SpriteMapper::getInstance()) {}
};