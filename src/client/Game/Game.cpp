//
// Created by roussierenoa on 4/11/25.
//

#include "Game.hpp"

#include <iostream>

namespace jetpack::Client {
	void Game::display(sf::RenderWindow &window) {
		this->_background.display(window);
	}

	void Game::compute() {
		if (this->_clock.getElapsedTime().asMilliseconds() >= 25) {
			this->_clock.restart();
			auto pos = this->_background.getPos();
			if (pos.x > -2000)
				this->_background.changePos({(float)(pos.x - 10), pos.y});
			// Else  {// end of map}
		}
	}

	void Game::_handleKeyPressed(const sf::Event &event) {
		if (event.key.code == sf::Keyboard::Up) {
			this->_sendUserInteraction(UserInteractions_s::UP);
		}
		// TODO(noa) : demander les autres interaction possible entre le
		// server et le client
		if (event.key.code == sf::Keyboard::Escape) {
			this->_sendUserInteraction(UserInteractions_s::ESCAPE);
		}
	}

	void Game::analyze(const sf::Event &event) {
		if (event.type == sf::Event::KeyPressed)
			this->_handleKeyPressed(event);
	}

	Game::Game(std::function<void(UserInteractions_s)> &sendUserInteraction) :
		_sendUserInteraction(sendUserInteraction),
		_background("src/client/assets/GameBackground.png", {0, 0}, {2, 2.1})
	{
		this->_clock = sf::Clock();
	}

	Game::~Game() {
	}
} // jetpack