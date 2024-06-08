#include "SpriteServer/pojo/User.hpp"

User::User(std::string name, std::string password, int winner, int battleTimes)
    : name(name), password(password), winner(winner), battleTimes(battleTimes)
{
}

void User::showInfo() const
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Password: " << password << std::endl;
    std::cout << "Battle Times: " << battleTimes << std::endl;
    std::cout << "Winner: " << winner << std::endl;
    std::cout << "Medals: ";
    for (auto medal : medals)
    {
        std::cout << medal << " ";
    }
    std::cout << std::endl;
    std::cout << "Sprites: " << std::endl;
    for (auto sprite : sprites)
    {
        sprite.showInfo();
    }
}

std::string User::getName() const {
    return name;
}

bool User::checkPassword(std::string password) const
{
    return this->password == password;
}

std::string User::getPassword() const {
    return password;
}

double User::getRate() const {
    return winner * 1.0 / battleTimes;
}

void User::setPassword(std::string newPassword) {
    password = newPassword;
}

void User::setName(std::string newName) {
    name = newName;
}

void User::addMedal(std::string medal) {
    medals.push_back(medal);
}

void User::addSprite(Sprite sprite) {
    sprites.push_back(sprite);
}

void User::removeSprite(std::string spriteName) {
    for (auto it = sprites.begin(); it != sprites.end(); it++)
    {
        if (it->getSpriteName() == spriteName)
        {
            sprites.erase(it);
            break;
        }
    }
}

void User::showMedals() {
    for (auto medal : medals)
    {
        std::cout << medal << " ";
    }
    std::cout << std::endl;
}

nlohmann::json User::toJson(const User &user) {
    nlohmann::json j;
    j["name"] = user.name;
    j["password"] = user.password;
    j["battleTimes"] = user.battleTimes;
    j["winner"] = user.winner;
    j["medals"] = user.medals;
    j["sprites"] = nlohmann::json::array();
    for (auto sprite : user.sprites) {
        j["sprites"].push_back(Sprite::toJson(sprite));
    }
    return j;
}

std::shared_ptr<User> User::fromJson(const nlohmann::json &j) {
    std::string name = j["name"];
    std::string password = j["password"];
    double rate = j["rate"];
    int battleTimes = j["battleTimes"];
    std::vector<std::string> medals = j["medals"];
    std::vector<Sprite> sprites;
    for (auto sprite : j["sprites"])
    {
        sprites.push_back(*Sprite::fromJson(sprite));
    }
    auto user = std::make_shared<User>(name, password, rate);
    user->setMedals(medals);
    user->setSprites(sprites);
    user->setBattleTimes(battleTimes);
    return user;
}

void User::setSprites(std::vector<Sprite> sprites) {
    this->sprites = sprites;
}

void User::setMedals(std::vector<std::string> medals) {
    this->medals = medals;
}

void User::setBattleTimes(int battleTimes) {
    this->battleTimes = battleTimes;
}

int User::getBattleTimes() const {
    return battleTimes;
}

void User::addWinner() {
    winner++;
    battleTimes++;
}

int User::getWinner() const {
    return winner;
}

void User::addBattleTimes() {
    battleTimes++;
}
