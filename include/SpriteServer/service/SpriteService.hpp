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
};