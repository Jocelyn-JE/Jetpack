//
// Created by roussierenoa on 4/12/25.
//

#include "Laser.hpp"
#include <iostream>

namespace jetpack {
namespace Client {
	void Laser::display(sf::RenderWindow &window) {
		this->_laserSheet.display(window);
	}

	void Laser::compute() {
		this->_laserMutex.lock();
		this->_laserSheet.setPos(this->_pos);
		if (this->_clock.getElapsedTime().asMilliseconds() > 200) {
			this->_laserSheet.nextSprite();
			this->_clock.restart();
		}
		this->_laserMutex.unlock();
	}

	void Laser::changePosValue(const sf::Vector2f &pos) {
		this->_laserMutex.lock();
		this->_pos = pos;
		this->_laserMutex.unlock();
	}

	sf::Vector2f Laser::getPos() const {
		return this->_laserSheet.getSpritePosition();
	}

	Laser::Laser() : _laserSheet("./src/client/assets/laser_sprite_sheet.png", {106, 120}, 1, 4, {1, 1}) {
		this->_pos = {0, 0};
		this->_clock.restart();
	}

} // Client
} // jetpack