//
// Created by roussierenoa on 4/12/25.
//

#include "Coin.hpp"

#include <iostream>

namespace jetpack {
namespace Client {
	void Coin::display(sf::RenderWindow &window) {
		this->_coinSheet.display(window);
	}

	void Coin::compute() {
		this->_coinMutex.lock();
		this->_coinSheet.setPos(this->_pos);
		if (this->_clock.getElapsedTime().asMilliseconds() > 100) {
			this->_coinSheet.nextSprite();
			this->_clock.restart();
		}
		this->_coinMutex.unlock();
	}

	void Coin::changePosValue(const sf::Vector2f &pos) {
		this->_coinMutex.lock();
		this->_pos = pos;
		this->_coinMutex.unlock();
	}

	sf::Vector2f Coin::getPos() const {
		return this->_coinSheet.getSpritePosition();
	}

	Coin::Coin() : _coinSheet("./src/client/assets/coins_sprite_sheet.png", {193, 170}, 1, 6, {0.2, 0.2}) {
		this->_pos = {0, 0};
		this->_clock.restart();
	}

} // Client
} // jetpack