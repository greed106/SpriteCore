#include "SpriteServer/controller/HttpController.hpp"
#include "SpriteServer/controller/BattleController.hpp"

int main() {
    Mapper::initializePool("host=localhost;user=root;password=ZHYzhy.0310;port=3306;db=sprite_server");
    int port = 8080;
    hv::HttpService router;
    HttpController::registerRoutes(router);
    hv::WebSocketService ws;
    BattleController::registerWebSocketService(ws);

    hv::WebSocketServer server;
    server.setPort(port);
    server.registerHttpService(&router);
    server.registerWebSocketService(&ws);

    server.run(false);
    std::cout << "Server started at port " << port << std::endl;
    std::string str;
    while(true){
        std::cin >> str;
        if(str == "quit"){
            break;
        }else{
            std::cout << "Unknown command: " << str << std::endl;
        }
    }

    return 0;
}

//#include "SpriteBattle/SpriteBattle.hpp"
//#include "SpriteCore/Factory.hpp"
//#include "SpriteServer/pojo/JsonSprite.hpp"
//
//int main() {
//    JsonSprite sprite("class Primeape", "monkey");
//    BattleRequest req("ymj", sprite, sprite, false, "opponent");
//
//    nlohmann::json j = req;
//    std::cout << j.dump(2) << std::endl;
//
//    return 0;
//}