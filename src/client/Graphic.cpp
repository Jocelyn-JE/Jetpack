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
			std::cout << "Displaying player " << s.first << std::endl;
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
			std::cout << "Compute player " << s.first << std::endl;
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

void jetpack::Client::Graphic::setUsername(const std::string &name) {
	this->_posMutex.lock();
	this->_menu.setUsername(name);
	this->_posMutex.unlock();
}

std::string jetpack::Client::Graphic::getUsername() {
	this->_posMutex.lock();
	auto data = this->_menu.getUsername();
	this->_posMutex.unlock();
	return data;
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



void jetpack::Client::Graphic::addNewPlayer(unsigned int id, bool isCurrent) {
	(void) isCurrent;

	if (!this->_listPlayers.contains(id)) {
		this->_listPlayers.emplace(id, std::make_pair(
			std::make_unique<Player>(new Player(isCurrent)),
			sf::Vector2f(0, 0)
		));
	}
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
	std::function<void()> &sendChangeUserName
):
	_window(sf::VideoMode({1440, 550}), "Jetpack Joyride", sf::Style::Close | sf::Style::Titlebar),
	_sendUserEvent(sendUserInteraction),
	_sendChangeUserName(sendChangeUserName),
	_menu(sendChangeUserName),
	_game(sendUserInteraction)
{
	this->_windowType = GAME;
	this->_window.setFramerateLimit(144);
	this->addNewPlayer(1, true);
}
