//
// Created by roussierenoa on 4/11/25.
//

#include "Player.hpp"

namespace jetpack::Client {
	void Player::display(sf::RenderWindow &window) {
		this->_player.display(window);
	}

	void Player::compute() {
		this->_playerMutex.lock();
		this->_player.setSprite(1, 1);
		this->_player.setPos(this->_pos);
		this->_playerMutex.unlock();
	}

	void Player::changePosValue(sf::Vector2f pos) {
		this->_playerMutex.lock();
		this->_pos = pos;
		this->_playerMutex.unlock();
	}

	Player::Player(bool isPlayer)
		:
		_player("./src/client/assets/player_sprite_sheet.png", {130, 130}, 4, 6, {0.65, 0.65})
	{
		this->_pos = {0, 0};
		if (isPlayer)
			this->_player.setTransparency();
	}

	Player::~Player() {
	}
} // jetpack