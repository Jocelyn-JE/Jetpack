#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "Coin.hpp"
#include "GamePlayer.hpp"
#include "Obstacle.hpp"

struct GameData {
    std::mutex dataMutex;
    std::map<int, std::shared_ptr<gameplayer_t>> players;
    std::vector<std::shared_ptr<coinsPos_t>> coins;
    std::vector<std::shared_ptr<obstacle_t>> obstacles;
    double advancement = 0;
    size_t gameSpeed = 500;
    bool isRunning;
    bool debug;
    int port;
    std::string filename;

    GameData()
        : isRunning(false), debug(false), port(4242), filename("map.joy") {}
};
