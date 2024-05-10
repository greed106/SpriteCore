#include "sprite/Stirps.hpp"


Primeape::Primeape(int level) : 
    Stirps(new PowerType(15, 6, 60, 5), "Primeape")
{
    while(level > 1)
    {
        type->levelUp();
        level--;
    }
}

int Primeape::doAttack() const
{
    return type->getAttack();
}

void Primeape::enhance()
{
    type->levelUp();
}

Stirps::Stirps(Type *type, std::string stirpsName) : type(type), stirpsName(stirpsName)
{
}

bool Stirps::hurt(int damage)
{
    damage -= type->getDefense();
    damage = damage < 0 ? 0 : damage;
    if(type->getHealth() <= damage){
        type->setHealth(0);
        return true;
    }
    type->setHealth(type->getHealth() - damage);
    return false;
}

void Stirps::showInfo() const
{
    std::cout << "Stirps: " << stirpsName << std::endl;
    type->showInfo();
}

Stirps* Primeape::clone()
{
    return new Primeape(type->clone(), stirpsName);
}

Primeape::Primeape(Type *type, std::string stirpsName) : Stirps(type, stirpsName)
{
}