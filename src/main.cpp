//#include "SpriteServer/controller/HttpController.hpp"
//
//int main() {
//    Mapper::initializePool("host=localhost;user=root;password=ZHYzhy.0310;port=3306;db=sprite_server");
//    int port = 8080;
//    hv::HttpService router;
//    HttpController::registerRoutes(router);
//    hv::HttpServer server(&router);
//    server.setPort(port);
//    std::cout << "Server started at port " << port << std::endl;
//    server.run(true);
//
//
//    return 0;
//}

#include "SpriteBattle/SpriteBattle.hpp"
#include "SpriteCore/Factory.hpp"

int main() {
    BattleNode user1("ymj", *Factory::createRandomSprite());
    BattleNode user2("ymjjj", *Factory::createRandomSprite("", 2));
    auto result = SpriteBattle::battle(user1, user2);

    result.showInfo();

    return 0;
}
