#include <iostream>
#include "SpriteCore/Sprite.hpp"

int main(){
    Sprite sprite(new Primeape(), "tim");
    sprite.addExp(10000);
    sprite.showInfo();
    return 0;
}