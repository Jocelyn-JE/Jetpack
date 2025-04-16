//
// Created by roussierenoa on 4/11/25.
//

#include "GameGraphic.hpp"

#include <iostream>
#include <vector>

void jetpack::Client::Game::display(sf::RenderWindow &window,
    std::vector<sf::Vector2f> posCoin, std::vector<sf::Vector2f> posLaser) {
    this->_background.display(window);
    this->_background2.display(window);
    this->_coinStats.display(window);
    for (auto s : posCoin) {
        this->_coin.changePosValue(s);
        this->_coin.display(window);
    }
    for (auto s : posLaser) {
        this->_laser.changePosValue(s);
        this->_laser.display(window);
    }
}

void jetpack::Client::Game::compute() {
    if (this->_clock.getElapsedTime().asMilliseconds() >= 15) {
        this->_clock.restart();
        auto pos1 = this->_background.getPos();
        auto pos2 = this->_background2.getPos();

        this->_background.changePos({pos1.x - 5, pos1.y});
        this->_background2.changePos({pos2.x - 5, pos2.y});
        float bgWidth = 3444;
        float completelyOutPosition = -3452;
        if (pos1.x < completelyOutPosition)
            this->_background.changePos({pos2.x + bgWidth - 5, pos1.y});
        if (pos2.x < completelyOutPosition)
            this->_background2.changePos({pos1.x + bgWidth - 5, pos2.y});
        this->_coin.compute();
        this->_laser.compute();
    }
    this->_coinStats.compute();
}

void jetpack::Client::Game::setCoinsAmount(unsigned int coinsAmount) {
    this->_coinStats.setCoinsAmount(coinsAmount);
}

void jetpack::Client::Game::_handleKeyPressed(const sf::Event &event) {
    if (event.key.code == sf::Keyboard::Up) {
        this->_sendUserInteraction(UserInteractions_s::UP);
    }
    // TODO(noa) : demander les autres interaction possible entre le
    // server et le client
}

void jetpack::Client::Game::analyze(const sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) this->_handleKeyPressed(event);
}

jetpack::Client::Game::Game(
    std::function<void(UserInteractions_s)> &sendUserInteraction)
    : _sendUserInteraction(sendUserInteraction),
      _background("src/client/assets/GameBackground.png", {0, 0}, {2, 2.15}),
      _background2("src/client/assets/GameBackground.png", {3444, 0},
                   {2, 2.15}) {
    this->_clock = sf::Clock();
}

jetpack::Client::Game::~Game() {}
