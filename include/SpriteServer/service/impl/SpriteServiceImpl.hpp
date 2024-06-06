#pragma once

#include "SpriteServer/service/SpriteService.hpp"

class SpriteServiceImpl : public SpriteService {
public:
    static SpriteServiceImpl& getInstance() {
        static SpriteServiceImpl instance;
        return instance;
    }

    std::vector<Sprite> getSprites(std::string name) override {
        return SpriteMapper::getInstance().getSpritesByName(name);
    }
};