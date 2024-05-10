#include <iostream>
#include "sprite/Sprite.hpp"

int main(){
    Sprite sprite(new Primeape(), "tim");
    sprite.addExp(10000);
    sprite.showInfo();
    sprite.hurt(206);
    sprite.showInfo();
    return 0;
}