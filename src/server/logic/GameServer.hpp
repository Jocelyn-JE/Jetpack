#pragma once
#include <memory>
#include <string>

#include "../include/GameData.hpp"

class GameServer {
 private:
    int port;
    std::shared_ptr<GameData> gameData;
    void handleNewConnection();
    void handleMessage(const std::string &message);

 public:
    GameServer(int port, std::shared_ptr<GameData> data);
    void start();
};
