//
// Created by roussierenoa on 4/18/25.
//

#include "EndGame.hpp"
#include <map>
#include <string>
#include <utility>
#include <vector>

void jetpack::Client::EndGame::resetEndGameClock() {
    this->_endGameClock.restart();
}

void jetpack::Client::EndGame::display(sf::RenderWindow &window) {
    window.draw(this->_text);
}

void jetpack::Client::EndGame::compute(int id,  const std::map<unsigned int,
    std::pair<std::unique_ptr<Player>, sf::Vector2f>> &listPlayer) {
    if (this->_endGameClock.getElapsedTime().asSeconds() <= 5) {
        this->_switchToMenu();
    }
    std::vector<unsigned int> idDeadPlayer = {};
    for (auto &s : listPlayer) {
        if (s.second.first->isDead() == true)
            idDeadPlayer.push_back(s.first);
    }
    if (!idDeadPlayer.empty()) {
        bool isCurrentPlayerDead = std::find(idDeadPlayer.begin(),
            idDeadPlayer.end(), id) != idDeadPlayer.end();

        if (isCurrentPlayerDead)
            this->_text.setString("You Lost!");
        else
            this->_text.setString("You Won!");
    } else {
        this->_text.setString("Draw!");
    }
}

jetpack::Client::EndGame::EndGame(std::function<void()> &switchToMenu) :
    _switchToMenu(switchToMenu) {
    if (!this->_font.loadFromFile("src/client/assets/JetpackFont.ttf")) {
        throw std::runtime_error("Erreur : Impossible de charger la police"
            "JetpackFont.ttf");
    }
    this->_text.setCharacterSize(50);
    this->_text.setFillColor(sf::Color::White);
    this->_text.setFont(this->_font);
}

jetpack::Client::EndGame::~EndGame() {
}
