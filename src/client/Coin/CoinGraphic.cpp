//
// Created by roussierenoa on 4/12/25.
//

#include "CoinGraphic.hpp"

#include <iostream>

void jetpack::Client::Coin::display(sf::RenderWindow &window) {
    this->_coinSheet.display(window);
}

void jetpack::Client::Coin::compute() {
    this->_coinMutex.lock();
    if (this->_clock.getElapsedTime().asMilliseconds() > 100) {
        this->_coinSheet.nextSprite();
        this->_clock.restart();
    }
    this->_coinMutex.unlock();
}

void jetpack::Client::Coin::changePosValue(const sf::Vector2f &pos) {
    this->_coinMutex.lock();
    this->_coinSheet.setPos(pos);
    this->_coinMutex.unlock();
}

sf::Vector2f jetpack::Client::Coin::getPos() const {
    return this->_coinSheet.getSpritePosition();
}

jetpack::Client::Coin::Coin()
    : _coinSheet("./src/client/assets/coins_sprite_sheet.png", {193, 170}, 1, 6,
                 {0.2, 0.2}) {
    this->_pos = {0, 0};
    this->_clock.restart();
}
