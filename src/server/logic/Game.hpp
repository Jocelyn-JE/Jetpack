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
    // 50ms per tick = 20 ticks per second
    static constexpr float TICK_RATE = 50.0f;
    static constexpr float GRAVITY = 40.00f;
    static constexpr float JETPACK_FORCE = 400.0f;
    static constexpr float VMAX = -5.0f;
    static constexpr float VMIN = 4.0f;
    Clock clock;
    std::shared_ptr<GameData> gameData;
    // Ncurses window for the map display
    WINDOW* mapWin = nullptr;

    // Check for collisions between players and coins/obstacles
    // This function iterates through all players and checks if they are
    // colliding with any coins or obstacles. If a player collects a coin,
    // the coin is removed from the game. If a player collides with an
    // obstacle, the player is marked as dead.
    void checkCollisions();
    // Load the map from a file
    // This function uses the MapParser to load the map from a file. It
    // populates the game data with the coins and obstacles found in the
    // map file (filename).
    bool loadMap(const std::string& filename);
    // Initialize ncurses map
    void initNcursesMap();

 public:
    // Constructor
    // Initializes the game with the given game data
    // The game data contains the players, coins, obstacles, and other game
    // information.
    explicit Game(std::shared_ptr<GameData> data);
    // Updates the ongoing game to the next tick
    void update(float deltaTime);
    // Loads the map
    void start(const std::string& mapFile);
    // Stops the game
    void stop();
    // Main game loop standalone
    // This function runs the game loop, updating the game state and checking
    // for collisions. This shouldn't be used in the server's main networking
    // thread loop. As it would cause the packets to hang.
    void gameLoop();
    // Display the current status of the map
    void displayNcursesMap();
    // Adds a player to the game
    void addPlayer(int id);
    // Adds a player to the game
    void addPlayer(const std::string& username);
    // Adds a player to the game
    void addPlayer(int id, const std::string& username);
    // Removes a player from the game
    void delPlayer(int id);
    // Removes a player from the game
    void delPlayer(const std::string& username);
    // Returns the number of players in the game
    size_t nbPlayer() const;
    // Returns true if the game is started
    // Returns false if the game is not started
    bool isStarted() const;
    // Returns the player with the given id
    std::shared_ptr<gameplayer_t> getPlayer(int id);
};
