//
// Created by roussierenoa on 3/28/25.
//

#include "Graphic.hpp"

#include <memory>
#include <string>
#include <utility>

#include "ProgramGraphic.hpp"

void jetpack::Client::Graphic::display() {
    this->_window.clear();
    this->_posMutex.lock();
    if (this->_windowType == MENU) this->_menu.display(this->_window);
    if (this->_windowType == GAME) {
        this->_game.display(this->_window, this->_posCoin, this->_posLaser);
        for (auto &s : this->_listPlayers) {
            if (!s.second.first->isDead())
                s.second.first->display(this->_window);
        }
        if (this->_isEndGame)
            this->_endGame.display(this->_window);
    }
    this->_posMutex.unlock();
    this->_window.display();
}

void jetpack::Client::Graphic::clearCoinPos() {
    this->_posMutex.lock();
    this->_posCoin.clear();
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::clearLaserPos() {
    this->_posMutex.lock();
    this->_posLaser.clear();
    this->_posMutex.unlock();
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
            if (static_cast<unsigned int>(this->_getIdWithAuth()) ==
                s.second.first->id)
                this->_game.setCoinsAmount(s.second.first->getCoinsAmount());
        }
        if (this->_isEndGame)
            this->_endGame.compute();
    }
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosPlayer(unsigned int id, sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_listPlayers.at(id).second = pos;
    this->_listPlayers.at(id).first->changePosValue(pos);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setCoinAmount(unsigned int id, int coins) {
    this->_posMutex.lock();
    this->_listPlayers.at(id).first->setCoinsAmount(coins);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosCoin(sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_posCoin.push_back(pos);
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosLaser(sf::Vector2f pos) {
    this->_posMutex.lock();
    this->_posLaser.push_back(pos);
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
            id, std::make_pair(std::make_unique<Player>(isTransparent, id),
                               sf::Vector2f(0, 0)));
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPlayerStatus(unsigned int id, bool isDead) {
    this->_listPlayers.at(id).first->setPlayerStatus(isDead);
}

void jetpack::Client::Graphic::switchToGame() { this->_windowType = GAME; }

void jetpack::Client::Graphic::switchToMenu() {
    this->_isEndGame = false;
    this->_windowType = MENU;
}

void jetpack::Client::Graphic::switchToDeath() {
    this->_endGame.resetEndGameClock();
    this->_endGame.setResult(this->_getIdWithAuth(), this->_listPlayers);
    this->_listPlayers.clear();
    this->_isEndGame = true;
}

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
    std::function<bool()> &getIsConnectedWithAuth,
    std::function<void()> &sendStartGame)
    : _window(sf::VideoMode({1440, 550}), "Jetpack Joyride",
              sf::Style::Close | sf::Style::Titlebar),
      _getIdWithAuth(getIdWithAuth),
      _menu(changeUsername, getUsername, getSocketSettings, sendSocketSetting,
            getIdWithAuth, getIsConnectedWithAuth, sendStartGame),
      _game(sendUserInteraction),
      _endGame(_switchToMenuFct) {
    this->_windowType = MENU;
    this->_window.setFramerateLimit(144);
}
