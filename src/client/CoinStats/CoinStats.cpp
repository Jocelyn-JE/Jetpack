//
// Created by roussierenoa on 4/12/25.
//

#include <iostream>
#include "CoinStats.hpp"

void jetpack::Client::CoinStats::setCoinsAmount(unsigned int coinsAmount) {
    this->_coinsAmount = coinsAmount;
}

void jetpack::Client::CoinStats::display(sf::RenderWindow &window) {
    this->_coinSprite.display(window);
    window.draw(this->_coinAmountText);
}

void jetpack::Client::CoinStats::compute() {
    auto newXPosition =
        1325 - ((std::to_string(this->_coinsAmount).length() - 1) * 20);
    this->_coinAmountText.setPosition({static_cast<float>(newXPosition), 15});
    this->_coinAmountText.setString(std::to_string(this->_coinsAmount));
}

jetpack::Client::CoinStats::CoinStats()
    : _coinSprite("./src/client/assets/coins_sprite_sheet.png", {193, 170}, 1,
                  1, {0.35, 0.35}) {
    this->_coinSprite.setPos({1350, 10});
    this->_coinSprite.setSprite(0, 0);

    if (!this->_jetpackFont.loadFromFile("src/client/assets/"
                                         "JetpackFont.ttf")) {
        throw std::runtime_error(
            "Erreur : Impossible de charger la police"
            " JetpackFont.ttf");
    }

    this->_coinAmountText.setFont(this->_jetpackFont);
    this->_coinAmountText.setString(std::to_string(this->_coinsAmount));
    this->_coinAmountText.setCharacterSize(40);
    this->_coinAmountText.setFillColor(sf::Color(255, 197, 84, 255));
    this->_coinAmountText.setOutlineThickness(3);
    this->_coinAmountText.setOutlineColor(sf::Color::Black);
    this->_coinAmountText.setPosition({1325, 15});
}
