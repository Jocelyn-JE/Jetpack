#include "Game.hpp"
#include <ncurses.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "../parsing/Parser.hpp"

Game::Game(std::shared_ptr<GameData> data) : gameData(data), mapWin(nullptr) {
    std::cerr << "Game constructor called" << std::endl;
}

void Game::start(const std::string& mapFile) {
    gameData->isRunning = true;
    gameData->advancement = 0;
    std::cerr << "Game started" << std::endl;

    if (!loadMap(mapFile)) {
        std::cerr << "Failed to load map: " << mapFile << std::endl;
        gameData->isRunning = false;
        return;
    }
    std::cerr << "Map loaded successfully" << std::endl;

    //initNcursesMap();
    // printServerData();
}

void Game::pollInput() {
    int ch = getch();  // Non-blocking input
    if (ch == 'w' || ch == 'W') {
        std::lock_guard<std::mutex> lock(gameData->dataMutex);
        if (!gameData->players.empty()) {
            auto& firstPlayer = gameData->players.begin()->second;
            if (!firstPlayer->is_dead) {
                firstPlayer->is_jetpack_on = true;
            }
        }
    } else if (ch == ERR) {
        // No input, reset jetpack state
        std::lock_guard<std::mutex> lock(gameData->dataMutex);
        if (!gameData->players.empty()) {
            auto& firstPlayer = gameData->players.begin()->second;
            firstPlayer->is_jetpack_on = false;
        }
    }
}

void Game::gameLoop() {
    std::cerr << "Game loop started" << std::endl;
    while (gameData->isRunning) {
        std::cerr << "Game loop iteration" << std::endl;
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
    // std::cerr << "Update called with deltaTime: " << deltaTime << std::endl;
    {
        std::lock_guard<std::mutex> lock(gameData->dataMutex);

        gameData->advancement += deltaTime / gameData->gameSpeed;
        gameData->advancement += deltaTime;
        for (auto& [_, player] : gameData->players) {
            if (player->is_dead) continue;

            // float verticalForce = player->is_jetpack_on ? -JETPACK_FORCE :
            // GRAVITY;
            player->velocity +=
                player->is_jetpack_on ? -JETPACK_FORCE * deltaTime : 0;
            player->velocity += GRAVITY * deltaTime;
            if (player->velocity > VMIN) {
                player->velocity = VMIN;
            } else if (player->velocity < VMAX) {
                player->velocity = VMAX;
            }
            player->y_pos += player->velocity * deltaTime;
            /*             std::cerr << "Player " << player->username
                                  << " position: " << player->y_pos
                                  << " velocity: " << player->velocity <<
               std::endl; */
            if (player->y_pos < 0.0) {
                player->y_pos = 0.0;
            } else if (player->y_pos > 10.0) {
                player->y_pos = 10.0;
            }
        }
        checkCollisions();
    }
    //pollInput();
    // printServerData();
    // displayNcursesMap();
}

void Game::checkCollisions() {
    for (auto& [_, player] : gameData->players) {
        if (player->is_dead) continue;

        for (auto it = gameData->coins.begin(); it != gameData->coins.end();) {
            auto& coin = *it;
            if (coin->y_pos <= player->y_pos + 1.0 &&
                coin->y_pos + 1.0 >= player->y_pos &&
                coin->x_pos <= gameData->advancement + 1.0 &&
                coin->x_pos + 1.0 >= gameData->advancement) {
                player->coins_collected++;
                it = gameData->coins.erase(it);
            } else {
                ++it;
            }
        }

        for (const auto& obstacle : gameData->obstacles) {
            if (obstacle->y_pos + 1.0 >= player->y_pos &&
                obstacle->y_pos <= player->y_pos + 1.0 &&
                obstacle->x_pos <= gameData->advancement + 1.0 &&
                obstacle->x_pos + 1.0 >= gameData->advancement) {
                player->is_dead = true;
                break;
            }
        }
    }
}

void Game::stop() {
    gameData->isRunning = false;
    if (mapWin) {
        delwin(mapWin);
        endwin();
        mapWin = nullptr;
    }
}

bool Game::loadMap(const std::string& filename) {
    return jetpack::MapParser::parseMap(filename, gameData->coins,
                                        gameData->obstacles);
}

void Game::printServerData() const {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);

    /*     std::cerr << "\n=== Server Data ===" << std::endl;

        std::cerr << "Advancement: " << gameData->advancement << std::endl;
        std::cerr << "Game Speed: " << gameData->gameSpeed << std::endl;

        // DEBUUG Print Players
        std::cerr << "\nPlayers (" << gameData->players.size() << "):" <<
       std::endl; for (const auto& [id, player] : gameData->players) { std::cerr
       << "Player ID: " << id << "\n  Username: " << player->username
                      << "\n  Position Y: " << player->y_pos
                      << "\n  Coins: " << player->coins_collected
                      << "\n  Status: " << (player->is_dead ? "Dead" : "Alive")
                      << "\n  Jetpack: " << (player->is_jetpack_on ? "On" :
       "Off")
                      << std::endl;
        } */

    //// Print Coins
    // std::cerr << "\nCoins (" << gameData->coins.size() << "):" << std::endl;
    // for (const auto& coin : gameData->coins) {
    //     std::cerr << "Coin at position (" << coin->x_pos << ", " <<
    //     coin->y_pos
    //               << ")" << std::endl;
    // }

    //// Print Obstacles
    // std::cerr << "\nObstacles (" << gameData->obstacles.size()
    //           << "):" << std::endl;
    // for (const auto& obstacle : gameData->obstacles) {
    //     std::cerr << "Obstacle at position (" << obstacle->x_pos << ", "
    //               << obstacle->y_pos << ")" << std::endl;
    // }

    // std::cerr << "\n=================" << std::endl;
}

void Game::initNcursesMap() {
    initscr();              // Initialize ncurses
    noecho();               // Disable echoing of typed characters
    curs_set(0);            // Hide the cursor
    nodelay(stdscr, TRUE);  // Make getch() non-blocking

    mapWin = newwin(12, 120, 0, 0);  // Create a 10x120 window
    box(mapWin, 0, 0);               // Draw a border around the window
    wrefresh(mapWin);                // Refresh the window to show the border
}

void Game::displayNcursesMap() {
    if (!mapWin) return;  // Ensure the window is initialized

    werase(mapWin);     // Clear the window
    box(mapWin, 0, 0);  // Redraw the border

    {
        std::lock_guard<std::mutex> lock(gameData->dataMutex);

        // Draw coins
        for (const auto& coin : gameData->coins) {
            int x = static_cast<int>(coin->x_pos);
            int y = static_cast<int>(coin->y_pos);
            if (y >= 0 && y < 10 && x >= 0 && x < 120) {
                mvwaddch(mapWin, y + 1, x, '*');
            }
        }

        // Draw obstacles
        for (const auto& obstacle : gameData->obstacles) {
            int x = static_cast<int>(obstacle->x_pos);
            int y = static_cast<int>(obstacle->y_pos);
            if (y >= 0 && y < 10 && x >= 0 && x < 120) {
                mvwaddch(mapWin, y + 1, x, '#');
            }
        }

        // Draw players
        for (const auto& [_, player] : gameData->players) {
            if (!player->is_dead) {
                int x = static_cast<int>(gameData->advancement);
                int y = static_cast<int>(player->y_pos);
                if (y >= 0 && y < 10 && x >= 0 && x < 120) {
                    mvwaddch(mapWin, y + 1, x, 'P');
                }
            }
        }
    }

    wrefresh(mapWin);  // Refresh the window to show changes

    // Display velocity and position of the first two players below the map
    int row = 12;  // Start below the map window
    int col = 0;
    int playerCount = 0;

    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    for (const auto& [_, player] : gameData->players) {
        mvprintw(row++, col, "Player %s: Position Y = %.2f, Velocity = %.2f",
                 player->username, player->y_pos, player->velocity);
        playerCount++;
    }
    refresh();  // Refresh the standard screen to show the text
}

void Game::addPlayer(const std::string& username) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    int id =
        gameData->players.empty() ? 1 : gameData->players.rbegin()->first + 1;
    auto player = std::make_shared<gameplayer_t>(id, username);
    gameData->players[id] = player;
}

void Game::addPlayer(int id) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    auto player =
        std::make_shared<gameplayer_t>(id, "Player" + std::to_string(id));
    gameData->players[id] = player;
}

void Game::addPlayer(int id, const std::string& username) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    auto player = std::make_shared<gameplayer_t>(id, username);
    gameData->players[id] = player;
}

void Game::delPlayer(const std::string& username) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    for (auto it = gameData->players.begin(); it != gameData->players.end();
         ++it) {
        if (it->second->username == username) {
            gameData->players.erase(it);
            break;
        }
    }
}

void Game::delPlayer(int id) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    gameData->players.erase(id);
}

size_t Game::nbPlayer() const {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    return gameData->players.size();
}

bool Game::isStarted() const { return gameData->isRunning; }

std::shared_ptr<gameplayer_t> Game::getPlayer(int id) {
    std::lock_guard<std::mutex> lock(gameData->dataMutex);
    auto it = gameData->players.find(id);
    if (it != gameData->players.end()) {
        return it->second;
    }
    return nullptr;
}
