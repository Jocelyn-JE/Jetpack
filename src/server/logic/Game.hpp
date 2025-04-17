/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Game
*/

#pragma once
#include <ncurses.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../include/GameData.hpp"
#include "Clock.hpp"

class Game {
 private:
    static constexpr float TICK_RATE =
        50.0f;  // 50ms per tick = 20 ticks per second
    static constexpr float GRAVITY = 40.00f;
    static constexpr float JETPACK_FORCE = 400.0f;
    static constexpr float VMAX = -5.0f;
    static constexpr float VMIN = 4.0f;
    Clock clock;
    std::shared_ptr<GameData> gameData;
    WINDOW* mapWin = nullptr;  // Ncurses window for the map display

    void checkCollisions();
    bool loadMap(const std::string& filename);
    void printServerData() const;
    void initNcursesMap();  // Initialize ncurses map
    void pollInput();       // Poll for user input to control the jetpack

 public:
    void update(float deltaTime);
    explicit Game(std::shared_ptr<GameData> data);
    void start(const std::string& mapFile);
    void stop();
    void gameLoop();
    void displayNcursesMap();  // Display the current map
    void addPlayer(int id);
    void addPlayer(const std::string& username);
    void addPlayer(int id, const std::string& username);
    void delPlayer(int id);
    void delPlayer(const std::string& username);
    size_t nbPlayer() const;
    bool isStarted() const;
    std::shared_ptr<gameplayer_t> getPlayer(int id);
};
