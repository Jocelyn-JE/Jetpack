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
#include <ncurses.h>

#include "../../../include/Coin.hpp"
#include "../../../include/GameData.hpp"
#include "../../../include/Obstacle.hpp"
#include "../../../include/Player.hpp"
#include "Clock.hpp"

class Game {
 private:
    static constexpr float TICK_RATE =
        50.0f;  // 50ms per tick = 20 ticks per second
    static constexpr float GRAVITY = 25.00f;
    static constexpr float JETPACK_FORCE = 150.0f;
    static constexpr float VMAX = -8.0f;
    static constexpr float VMIN = 4.0f;
    Clock clock;
    std::shared_ptr<GameData> gameData;
    WINDOW* mapWin = nullptr; // Ncurses window for the map display

    void update(float deltaTime);
    void checkCollisions();
    bool loadMap(const std::string& filename);
    void printServerData() const;
    void initNcursesMap();    // Initialize ncurses map
    void pollInput();         // Poll for user input to control the jetpack

 public:
    explicit Game(std::shared_ptr<GameData> data);
    void start(const std::string& mapFile);
    void stop();
    void gameLoop();
    void displayNcursesMap(); // Display the current map
};
