//
// Created by roussierenoa on 4/11/25.
//

#include "Player.hpp"
#include <iostream>

namespace jetpack::Client {
	void Player::display(sf::RenderWindow &window) {
		this->_player.display(window);
	}

	void Player::compute() {
		this->_playerMutex.lock();
		this->_player.setPos(this->_pos);
		if (this->_clock.getElapsedTime().asMilliseconds() > 150) {
			this->_player.nextSprite();
			if (this->_pos.y < 400 && this->_currentSpriteState != JETPACK_ON) {
				this->_player.setSpriteType(JETPACK_ON);
				this->_currentSpriteState = JETPACK_ON;
			}
			if (this->_pos.y >= 400 && this->_currentSpriteState != RUNNING) {
				this->_player.setSpriteType(RUNNING);
				this->_currentSpriteState = RUNNING;
			}
			this->_clock.restart();
		}
		this->_playerMutex.unlock();
	}

	void Player::changePosValue(sf::Vector2f pos) {
		this->_playerMutex.lock();
		this->_pos = pos;
		this->_playerMutex.unlock();
	}

	unsigned int Player::getCoinsAmount() {
		this->_playerMutex.lock();
		auto data =  this->_coinsAmount;
		this->_playerMutex.unlock();
		return data;
	}

	void Player::setCoinsAmount(unsigned int coins) {
		this->_playerMutex.lock();
		this->_coinsAmount = coins;
		this->_playerMutex.unlock();
	}

	Player::Player(bool isPlayer)
		:
		_isHost(!isPlayer),
		_player("./src/client/assets/player_sprite_sheet.png", {138, 135}, 6, 4, {0.65, 0.65})
	{
		this->_pos = {0, 0};
		if (isPlayer)
			this->_player.setTransparency();
		this->_clock.restart();
		this->_currentSpriteState = RUNNING;
		this->_player.setSpriteType(this->_currentSpriteState);
	}

	Player::~Player() {
	}
} // jetpack