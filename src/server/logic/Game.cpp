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
    this->gameData->isRunning = true;
    this->gameData->advancement = 0;
    std::cerr << "Game started" << std::endl;
    if (!loadMap(mapFile)) {
        std::cerr << "Failed to load map: " << mapFile << std::endl;
        this->gameData->isRunning = false;
        return;
    }
    std::cerr << "Map loaded successfully" << std::endl;
    {
        std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
        for (auto& [_, player] : this->gameData->players) {
            player->coins.clear();
            for (const auto& coin : this->gameData->coins) {
                player->coins.push_back(coin);
            }
        }
    }
    // initNcursesMap();
}

void Game::gameLoop() {
    std::cerr << "Game loop started" << std::endl;
    while (this->gameData->isRunning) {
        std::cerr << "Game loop iteration" << std::endl;
        auto startTime = std::chrono::high_resolution_clock::now();
        float deltaTime = clock.tick();
        this->update(deltaTime);
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                  startTime);
        auto sleepTime =
            std::chrono::milliseconds(static_cast<int>(TICK_RATE)) -
            elapsedTime;
        if (sleepTime > std::chrono::milliseconds(0))
            std::this_thread::sleep_for(sleepTime);
    }
}

void Game::update(float deltaTime) {
    {
        std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

        this->gameData->advancement += deltaTime / this->gameData->gameSpeed;
        this->gameData->advancement += deltaTime;
        for (auto& [_, player] : this->gameData->players) {
            if (player->is_dead) continue;
            player->velocity +=
                player->is_jetpack_on ? -JETPACK_FORCE * deltaTime : 0;
            player->velocity += GRAVITY * deltaTime;
            if (player->velocity > VMIN)
                player->velocity = VMIN;
            else if (player->velocity < VMAX)
                player->velocity = VMAX;
            player->y_pos += player->velocity * deltaTime;
            if (player->y_pos < 0.0)
                player->y_pos = 0.0;
            else if (player->y_pos > 10.0)
                player->y_pos = 10.0;
        }
        checkCollisions();
    }
    // displayNcursesMap();
}

void Game::checkCollisions() {
    for (auto& [_, player] : this->gameData->players) {
        if (player->is_dead) continue;
        for (auto it = player->coins.begin(); it != player->coins.end();) {
            auto& coin = *it;
            if (coin->y_pos <= player->y_pos + 1.0 &&
                coin->y_pos + 1.0 >= player->y_pos &&
                coin->x_pos <= this->gameData->advancement + 1.0 &&
                coin->x_pos + 1.0 >= this->gameData->advancement) {
                player->coins_collected++;
                it = player->coins.erase(it);
            } else {
                ++it;
            }
        }
        for (const auto& obstacle : this->gameData->obstacles) {
            if (obstacle->y_pos + 1.0 >= player->y_pos &&
                obstacle->y_pos <= player->y_pos + 1.0 &&
                obstacle->x_pos <= this->gameData->advancement + 1.0 &&
                obstacle->x_pos + 1.0 >= this->gameData->advancement) {
                player->is_dead = true;
                break;
            }
        }
    }
}

bool Game::checkEndgame() {
    int alivePlayers = 0;
    {
        std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
        for (const auto& [_, player] : this->gameData->players) {
            if (!player->is_dead) {
                alivePlayers++;
            }
        }
    }
    if (alivePlayers <= 1) {
        return true;
    }
    return false;
}

void Game::stop() {
    this->gameData->isRunning = false;
    if (this->mapWin) {
        delwin(this->mapWin);
        endwin();
        this->mapWin = nullptr;
    }
}

bool Game::loadMap(const std::string& filename) {
    return jetpack::MapParser::parseMap(filename, this->gameData->coins,
                                        this->gameData->obstacles);
}

void Game::initNcursesMap() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    this->mapWin = newwin(12, 120, 0, 0);
    box(this->mapWin, 0, 0);
    wrefresh(this->mapWin);
}

void Game::displayNcursesMap() {
    if (!this->mapWin) return;
    werase(this->mapWin);
    box(this->mapWin, 0, 0);
    {
        std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

        for (const auto& coin : this->gameData->coins) {
            int x = static_cast<int>(coin->x_pos);
            int y = static_cast<int>(coin->y_pos);
            if (y >= 0 && y < 10 && x >= 0 && x < 120)
                mvwaddch(this->mapWin, y + 1, x, '*');
        }
        for (const auto& obstacle : this->gameData->obstacles) {
            int x = static_cast<int>(obstacle->x_pos);
            int y = static_cast<int>(obstacle->y_pos);
            if (y >= 0 && y < 10 && x >= 0 && x < 120)
                mvwaddch(this->mapWin, y + 1, x, '#');
        }
        for (const auto& [_, player] : this->gameData->players) {
            if (!player->is_dead) {
                int x = static_cast<int>(this->gameData->advancement);
                int y = static_cast<int>(player->y_pos);
                if (y >= 0 && y < 10 && x >= 0 && x < 120)
                    mvwaddch(this->mapWin, y + 1, x, 'P');
            }
        }
    }
    wrefresh(this->mapWin);
    {
        int row = 12;
        std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

        for (const auto& [_, player] : this->gameData->players)
            mvprintw(row++, 0, "Player %s: Position Y = %.2f, Velocity = %.2f",
                     player->username, player->y_pos, player->velocity);
        refresh();
    }
}

void Game::addPlayer(const std::string& username) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
    int id = this->gameData->players.empty()
                 ? 1
                 : this->gameData->players.rbegin()->first + 1;
    auto player = std::make_shared<gameplayer_t>(id, username);

    this->gameData->players[id] = player;
}

void Game::addPlayer(int id) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
    auto player =
        std::make_shared<gameplayer_t>(id, "Player" + std::to_string(id));

    this->gameData->players[id] = player;
}

void Game::addPlayer(int id, const std::string& username) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
    auto player = std::make_shared<gameplayer_t>(id, username);

    this->gameData->players[id] = player;
}

void Game::delPlayer(const std::string& username) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

    for (auto it = this->gameData->players.begin();
         it != this->gameData->players.end(); ++it) {
        if (it->second->username == username) {
            this->gameData->players.erase(it);
            break;
        }
    }
}

void Game::delPlayer(int id) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

    this->gameData->players.erase(id);
}

size_t Game::nbPlayer() const {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);

    return this->gameData->players.size();
}

bool Game::isStarted() const { return this->gameData->isRunning; }

std::shared_ptr<gameplayer_t> Game::getPlayer(int id) {
    std::lock_guard<std::mutex> lock(this->gameData->dataMutex);
    auto it = this->gameData->players.find(id);

    if (it != this->gameData->players.end()) return it->second;
    return nullptr;
}
