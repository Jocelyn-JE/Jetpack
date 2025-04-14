#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include "Coin.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"

struct GameData {
    std::mutex dataMutex;
    std::map<int, std::shared_ptr<player_t>> players;
    std::vector<std::shared_ptr<coinsPos_t>> coins;
    std::vector<std::shared_ptr<obstacle_t>> obstacles;
    bool isRunning;
    bool debug;
    int port;
    std::string filename;

    GameData() : isRunning(true), debug(false), port(4242), filename("map.joy") {}
};
