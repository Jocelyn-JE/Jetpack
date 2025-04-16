//
// Created by roussierenoa on 3/28/25.
//

#include "Graphic.hpp"

#include <memory>
#include <string>
#include <utility>

#include "Program.hpp"

void jetpack::Client::Graphic::display() {
    this->_window.clear();
    if (this->_windowType == MENU) this->_menu.display(this->_window);
    if (this->_windowType == GAME) {
        this->_game.display(this->_window);
        for (auto &s : this->_listPlayers)
            s.second.first->display(this->_window);
        for (auto &s : this->_listCoins) s.second.first->display(this->_window);
        for (auto &s : this->_listLasers)
            s.second.first->display(this->_window);
    }
    this->_window.display();
}

void jetpack::Client::Graphic::compute() {
    this->_posMutex.lock();
    if (this->_windowType == MENU) {
        this->_menu.compute();
    }
    if (this->_windowType == GAME) {
        this->_game.compute();
        for (auto &s : this->_listPlayers) {
            s.second.first->compute();
            if (s.second.first->isHost())
                this->_game.setCoinsAmount(s.second.first->getCoinsAmount());
        }
        for (auto &s : this->_listCoins) {
            s.second.first->compute();
        }
        for (auto &s : this->_listLasers) {
            s.second.first->compute();
        }
    }
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosPlayer(unsigned int id, sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_listPlayers.at(id).second = pos;
    this->_listPlayers.at(id).first->changePosValue(pos);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosCoin(unsigned int id, sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_listCoins.at(id).second = pos;
    this->_listCoins.at(id).first->changePosValue(pos);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosLaser(unsigned int id, sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_listLasers.at(id).second = pos;
    this->_listLasers.at(id).first->changePosValue(pos);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::analyse() {
    sf::Event event{};
    while (this->_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) this->_window.close();
        if (this->_windowType == MENU)
            this->_menu.analyze(event);
        else
            this->_game.analyze(event);
    }
}

void jetpack::Client::Graphic::setGameSpeed(size_t value) {
    this->_posMutex.lock();
    this->_gameSpeed = value;
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::addNewPlayer(unsigned int id,
                                            bool isTransparent) {
    this->_posMutex.lock();
    if (!this->_listPlayers.contains(id))
        this->_listPlayers.emplace(
            id, std::make_pair(std::make_unique<Player>(isTransparent),
                               sf::Vector2f(0, 0)));
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::addNewCoin(unsigned int id) {
    this->_posMutex.lock();
    if (!this->_listCoins.contains(id))
        this->_listCoins.emplace(
            id, std::make_pair(std::make_unique<Coin>(), sf::Vector2f(0, 0)));
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::addNewLaser(unsigned int id) {
    this->_posMutex.lock();
    if (!this->_listLasers.contains(id))
        this->_listLasers.emplace(
            id, std::make_pair(std::make_unique<Laser>(), sf::Vector2f(0, 0)));
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::switchToGame() { this->_windowType = GAME; }

void jetpack::Client::Graphic::switchToMenu() { this->_windowType = MENU; }

void jetpack::Client::Graphic::serverError() {
    this->_posMutex.lock();
    this->_menu.setServerStateError();
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::serverOK() {
    this->_posMutex.lock();
    this->_menu.setServerStateOk();
    this->_posMutex.unlock();
}

jetpack::Client::Graphic::Graphic(
    std::function<void(UserInteractions_s)> &sendUserInteraction,
    std::function<void(std::string)> &changeUsername,
    std::function<std::string()> &getUsername,
    std::function<std::pair<std::string, std::string>()> &getSocketSettings,
    std::function<void(std::pair<std::string, int>)> &sendSocketSetting,
    std::function<int()> &getIdWithAuth,
    std::function<bool()> &getIsConnectedWithAuth)
    : _window(sf::VideoMode({1440, 550}), "Jetpack Joyride",
              sf::Style::Close | sf::Style::Titlebar),
      _menu(changeUsername, getUsername, getSocketSettings, sendSocketSetting,
            getIdWithAuth, getIsConnectedWithAuth),
      _game(sendUserInteraction) {
    this->_windowType = MENU;
    this->_window.setFramerateLimit(144);
}
