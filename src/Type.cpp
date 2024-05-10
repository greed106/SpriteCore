#include "sprite/Type.hpp"

Type::Type(int attack, int defense, int health,
     int attackSpeed, std::string typeName) :
        attack(attack), defense(defense), health(health),
        attackSpeed(attackSpeed), typeName(typeName)
{
}


int Type::getAttack() const {
    return this->attack;
}

int Type::getDefense() const {
    return this->defense;
}

int Type::getHealth() const {
    return this->health;
}

int Type::getAttackSpeed() const {
    return this->attackSpeed;
}

void Type::setAttack(int attack){
    this->attack = attack;
}

void Type::setDefense(int defense){
    this->defense = defense;
}

void Type::setHealth(int health){
    this->health = health;
}

void Type::setAttackSpeed(int attackSpeed){
    this->attackSpeed = attackSpeed;
}

std::string Type::getTypeName() const {
    return this->typeName;
}

void Type::showInfo() const {
    std::cout << "Type: " << this->typeName << std::endl;
    std::cout << "Attack: " << this->attack << std::endl;
    std::cout << "Defense: " << this->defense << std::endl;
    std::cout << "Health: " << this->health << std::endl;
    std::cout << "AttackSpeed: " << this->attackSpeed << std::endl;
}

SpeedType::SpeedType(int attack, int defense, int health, int attackSpeed) : 
    Type(attack, defense, health, attackSpeed, "Speed")
{
}

void SpeedType::levelUp(){
    this->attack += 5;
    this->defense += 5;
    this->health += 5;
    this->attackSpeed += 5;
}

PowerType::PowerType(int attack, int defense, int health, int attackSpeed) : 
    Type(attack, defense, health, attackSpeed, "Power")
{
}

void PowerType::levelUp(){
    this->attack += 8;
    this->defense += 5;
    this->health += 5;
    this->attackSpeed += 2;
}

DefenseType::DefenseType(int attack, int defense, int health, int attackSpeed) : 
    Type(attack, defense, health, attackSpeed, "Defense")
{
}

void DefenseType::levelUp(){
    this->attack += 5;
    this->defense += 8;
    this->health += 5;
    this->attackSpeed += 2;
}

TankType::TankType(int attack, int defense, int health, int attackSpeed) : 
    Type(attack, defense, health, attackSpeed, "Tank")
{
}

void TankType::levelUp(){
    this->attack += 5;
    this->defense += 5;
    this->health += 8;
    this->attackSpeed += 2;
}

Type* SpeedType::clone(){
    return new SpeedType(*this);
}

Type* PowerType::clone(){
    return new PowerType(*this);
}

Type* DefenseType::clone(){
    return new DefenseType(*this);
}

Type* TankType::clone(){
    return new TankType(*this);
}