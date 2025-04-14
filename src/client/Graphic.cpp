//
// Created by roussierenoa on 3/28/25.
//

#include "Graphic.hpp"
#include "Program.hpp"

void jetpack::Client::Graphic::display() {
    this->_window.clear();
    if (this->_windowType == MENU) {
        this->_menu.display(this->_window);
    }
    if (this->_windowType == GAME) {
        this->_game.display(this->_window);
        for (auto &s: this->_listPlayers) {
            s.second.first->display(this->_window);
        }
        for (auto &s: this->_listCoins) {
            s.second.first->display(this->_window);
        }
        for (auto &s: this->_listLasers) {
            s.second.first->display(this->_window);
        }
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
        for (auto &s: this->_listPlayers) {
            s.second.first->compute();
            if (s.second.first->isHost())
                this->_game.setCoinsAmount(s.second.first->getCoinsAmount());
        }
        for (auto &s: this->_listCoins) {
            s.second.first->compute();
        }
        for (auto &s: this->_listLasers) {
            s.second.first->compute();
        }
    }
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosPlayer(unsigned int id,
                                               sf::Vector2f pos) {
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
        if (event.type == sf::Event::Closed)
            this->_window.close();
        if (this->_windowType == MENU)
            this->_menu.analyze(event);
        else
            this->_game.analyze(event);
    }
}

void jetpack::Client::Graphic::addNewPlayer(unsigned int id, bool isTransparent) {
    if (!this->_listPlayers.contains(id))
        this->_listPlayers.emplace(id, std::make_pair(std::make_unique<Player>(isTransparent), sf::Vector2f(0, 0)));
}

void jetpack::Client::Graphic::addNewCoin(unsigned int id) {
    if (!this->_listCoins.contains(id))
        this->_listCoins.emplace(id, std::make_pair(std::make_unique<Coin>(), sf::Vector2f(0, 0)));
}

void jetpack::Client::Graphic::addNewLaser(unsigned int id) {
    if (!this->_listLasers.contains(id))
        this->_listLasers.emplace(id, std::make_pair(std::make_unique<Laser>(), sf::Vector2f(0, 0)));
}

void jetpack::Client::Graphic::switchToGame() {
    this->_windowType = GAME;
}

void jetpack::Client::Graphic::switchToMenu() {
    this->_windowType = MENU;
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
    std::function<std::string()> &getUsername
):
    _window(sf::VideoMode({1440, 550}), "Jetpack Joyride", sf::Style::Close | sf::Style::Titlebar),
    _sendUserEvent(sendUserInteraction),
    _changeUsername(changeUsername),
    _getUsername(getUsername),
    _menu(_changeUsername, _getUsername),
    _game(sendUserInteraction)
{
    this->_windowType = MENU;
    this->_window.setFramerateLimit(144);
    //Simulation de deux joueurs avec deux piece et deux laser
    this->addNewPlayer(1, false);
    this->addNewPlayer(2, true);
    this->addNewCoin(1);
    this->addNewCoin(2);
    this->addNewLaser(1);
    this->addNewLaser(2);
    this->setPosCoin(1, {1000, 400});
    this->setPosCoin(2, {1050, 400});
    this->setPosPlayer(1, {10, 400});
    this->setPosPlayer(2, {10, 200});
    this->setPosLaser(1, {500, 400});
    this->setPosLaser(2, {200, 200});
}
