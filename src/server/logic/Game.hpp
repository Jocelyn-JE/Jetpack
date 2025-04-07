/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Game
*/

#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../../include/Coin.hpp"
#include "../../../include/GameData.hpp"
#include "../../../include/Obstacle.hpp"
#include "../../../include/Player.hpp"
#include "Clock.hpp"

class Game {
 private:
    static constexpr float TICK_RATE =
        50.0f;  // 50ms per tick = 20 ticks per second
    static constexpr float GRAVITY = 9.81f;
    static constexpr float JETPACK_FORCE = 15.0f;
    Clock clock;
    std::shared_ptr<GameData> gameData;

    void update(float deltaTime);
    void checkCollisions();
    bool loadMap(const std::string& filename);
    void printServerData() const;

 public:
    explicit Game(std::shared_ptr<GameData> data);
    void start(const std::string& mapFile);
    void stop();
    void gameLoop();
};
