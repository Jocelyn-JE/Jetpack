//
// Created by roussierenoa on 4/18/25.
//

#include "EndGame.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

void jetpack::Client::EndGame::resetEndGameClock() {
    this->_endGameClock.restart();
}

void jetpack::Client::EndGame::display(sf::RenderWindow &window) {
    window.draw(this->_text);
}

void jetpack::Client::EndGame::setResult(unsigned int id,
    const std::map<unsigned int, std::pair<std::unique_ptr<Player>,
    sf::Vector2f>> &listPlayer) {
    std::vector<unsigned int> playersAlive;
    unsigned int maxCoins = 0;
    unsigned int playerCoins = 0;
    bool isDraw = false;

    for (const auto& [playerId, playerData] : listPlayer) {
        const auto &player = playerData.first;
        if (!player->isDead()) {
            playersAlive.push_back(playerId);
            unsigned int coins = player->getCoinsAmount();
            if (playerId == id)
                playerCoins = coins;
            if (coins > maxCoins) {
                maxCoins = coins;
                isDraw = false;
            } else {
                if (coins == maxCoins && coins > 0)
                    isDraw = true;
            }
        }
    }
    if (playersAlive.size() == 1) {
        if (playersAlive[0] == id)
            this->_text.setString("WIN");
        else
            this->_text.setString("LOSE");
    } else {
        if (playersAlive.size() > 1) {
            if (isDraw && playerCoins == maxCoins) {
                this->_text.setString("DRAW");
            } else {
                if (playerCoins == maxCoins)
                    this->_text.setString("WIN");
                else
                    this->_text.setString("LOSE");
            }
        } else {
            this->_text.setString("DRAW");
        }
    }
}

void jetpack::Client::EndGame::compute() {
    if (this->_endGameClock.getElapsedTime().asSeconds() >= 5)
        this->_switchToMenu();
}

jetpack::Client::EndGame::EndGame(std::function<void()> &switchToMenu) :
    _switchToMenu(switchToMenu) {
    if (!this->_font.loadFromFile("src/client/assets/JetpackFont.ttf")) {
        throw std::runtime_error("Erreur : Impossible de charger la police"
            "JetpackFont.ttf");
    }
    this->_text.setCharacterSize(50);
    this->_text.setPosition({250, 250});
    this->_text.setFillColor(sf::Color::White);
    this->_text.setFont(this->_font);
}

jetpack::Client::EndGame::~EndGame() {
}
