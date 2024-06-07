#pragma once

#include "SpriteServer/mapper/SpriteMapper.hpp"

class SpriteService {
protected:
    SpriteService() = default;
    virtual ~SpriteService() = default;
public:
    SpriteService(const SpriteService&) = delete;
    SpriteService& operator=(const SpriteService&) = delete;

    virtual std::vector<Sprite> getSprites(std::string name) = 0;
    virtual void addSprite(std::string username, const Sprite& sprite) = 0;
    virtual void removeSprite(std::string username, const std::string &spriteName) = 0;
    virtual void updateSprite(std::string username, const Sprite& sprite) = 0;
    virtual std::shared_ptr<Sprite> getSprite(std::string username, std::string spriteName) = 0;
    virtual bool isSpriteExist(std::string username, std::string spriteName) = 0;
};