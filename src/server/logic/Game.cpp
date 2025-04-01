#include "Game.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "MapParser.hpp"

Game::Game(std::shared_ptr<GameData> data) : gameData(data) {}

void Game::start(const std::string& mapFile) {
    gameData->isRunning = true;
    std::cout << "Game started" << std::endl;

    if (!loadMap(mapFile)) {
        std::cerr << "Failed to load map: " << mapFile << std::endl;
        gameData->isRunning = false;
        return;
    }
    std::cout << "Map loaded successfully" << std::endl;

    printServerData();
    gameLoop();
}

void Game::gameLoop() {
    while (gameData->isRunning) {
        auto startTime = std::chrono::high_resolution_clock::now();

        float deltaTime = clock.tick();
        update(deltaTime);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                  startTime);
        auto sleepTime =
            std::chrono::milliseconds(static_cast<int>(TICK_RATE)) -
            elapsedTime;

        if (sleepTime > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

void Game::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);

    for (auto& [_, player] : gameData->players) {
        if (player->is_dead) continue;

        float verticalForce = player->is_jetpack_on ? -JETPACK_FORCE : GRAVITY;
        player->y_pos += verticalForce * deltaTime;

        if (player->y_pos < 0) {
            player->y_pos = 0;
        } else if (player->y_pos > 100) {
            player->y_pos = 100;
        }
    }
    checkCollisions();
}

void Game::checkCollisions() {
    // Implement collision detection
}

void Game::stop() { gameData->isRunning = false; }

bool Game::loadMap(const std::string& filename) {
    return MapParser::parseMap(filename, gameData->coins, gameData->obstacles);
}

void Game::printServerData() const {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);

    std::cout << "\n=== Server Data ===" << std::endl;

    // Print Players
    std::cout << "\nPlayers (" << gameData->players.size() << "):" << std::endl;
    for (const auto& [id, player] : gameData->players) {
        std::cout << "Player ID: " << id << "\n  Username: " << player->username
                  << "\n  Position Y: " << player->y_pos
                  << "\n  Coins: " << player->coins_collected
                  << "\n  Status: " << (player->is_dead ? "Dead" : "Alive")
                  << "\n  Jetpack: " << (player->is_jetpack_on ? "On" : "Off")
                  << std::endl;
    }

    // Print Coins
    std::cout << "\nCoins (" << gameData->coins.size() << "):" << std::endl;
    for (const auto& coin : gameData->coins) {
        std::cout << "Coin at position (" << coin->x_pos << ", " << coin->y_pos
                  << ")" << std::endl;
    }

    // Print Obstacles
    std::cout << "\nObstacles (" << gameData->obstacles.size()
              << "):" << std::endl;
    for (const auto& obstacle : gameData->obstacles) {
        std::cout << "Obstacle at position (" << obstacle->x_pos << ", "
                  << obstacle->y_pos << ")" << std::endl;
    }

    std::cout << "\n=================" << std::endl;
}
