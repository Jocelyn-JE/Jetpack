//
// Created by roussierenoa on 3/28/25.
//

#include "Graphic.hpp"

#include "Program.hpp"

void jetpack::Client::Graphic::display() {
	this->_window.clear();
	if (this->_windowType == MENU) {
		this->_window.draw(this->_menuBackground);
		this->_window.draw(this->_menuCountdown);
		this->_window.draw(this->_usernameButton);
		this->_window.draw(this->_usernameTextButton);
		if (this->_isUserNamePressed) {
			this->_window.draw(this->_blurSprite);
			this->_window.draw(this->_usernameBox);
			this->_window.draw(this->_usernameBoxTitle);
			this->_window.draw(this->_usernameField);
			this->_window.draw(this->_usernameBoxContent);
		}
	}
	if (this->_windowType == GAME) {
		this->_window.draw(this->_gameBackground);
		for (auto &s: this->_listPlayers) {
			this->_window.draw(s.second.first);
		}
	}
	this->_window.display();
}

void jetpack::Client::Graphic::compute() {
	this->_posMutex.lock();
	if (this->_windowType == MENU) {
		// TODO(noa) : add menu
	}
	if (this->_windowType == GAME) {
		for (auto &s: this->_listPlayers) {
			s.second.first.setPosition(s.second.second);
		}
	}
	this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosRectangle(unsigned int id,
                                               sf::Vector2f pos) {
	this->_posMutex.lock();
	for (auto s: this->_listPlayers) this->_listPlayers.at(id).second = pos;
	this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setUsername(const std::string &name) {
	this->_posMutex.lock();
	this->_username = name;
	this->_posMutex.unlock();
}

void jetpack::Client::Graphic::_handleKeyPressed(const sf::Event &event) {
	if (event.key.code == sf::Keyboard::Up) {
		this->_sendUserEvent(UserInteractions_s::UP);
	}
	// TODO(noa) : demander les autres interaction possible entre le
	// server et le client
	if (event.key.code == sf::Keyboard::Escape) {
		this->_sendUserEvent(UserInteractions_s::ESCAPE);
	}
}

void jetpack::Client::Graphic::_handleMousePressed(const sf::Event &event) {
	if (this->_windowType == MENU) {
		if (event.mouseButton.button == sf::Mouse::Left) {
            if (this->_usernameButton.getGlobalBounds().contains(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y))) {
                this->_isUserNamePressed = !this->_isUserNamePressed;
            	if (this->_isUserNamePressed) {
            		this->_usernameTextButton.setString("Close");
            		this->_usernameButton.setSize({105, 70});
            	} else {
            		this->_sendChangeUserName();
            		this->_usernameTextButton.setString("Change Username");
            		this->_usernameButton.setSize({250, 70});
            	}
            }
        }
	}
}

void jetpack::Client::Graphic::analyse() {
	sf::Event event{};
	while (this->_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) this->_window.close();
		if (event.type == sf::Event::KeyPressed)
			this->_handleKeyPressed(event);
		if (event.type == sf::Event::MouseButtonPressed)
			this->_handleMousePressed(event);
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode >= 32 && event.text.unicode <= 126 && this->_username.length() < 20)
				this->_username += static_cast<char>(event.text.unicode);
			else if (event.text.unicode == 8 && !this->_username.empty())
				this->_username.pop_back();
			this->_usernameBoxContent.setString(this->_username);
		}
	}
}



void jetpack::Client::Graphic::addNewPlayer(unsigned int id, bool isCurrent) {
	(void) isCurrent;
	// Is current == is the player that is playing
	if (!this->_listPlayers.contains(id)) {
		sf::RectangleShape rect;
		rect.setSize({50.0f, 50.0f});
		rect.setFillColor(sf::Color::Red);
		sf::Vector2f pos = rect.getPosition();
		this->_listPlayers.insert({id, {rect, pos}});
	}
}

void jetpack::Client::Graphic::switchToGame() {
	this->_windowType = GAME;
}

void jetpack::Client::Graphic::switchToMenu() {
	this->_windowType = MENU;
}

jetpack::Client::Graphic::Graphic(
	std::function<void(UserInteractions_s)> &sendUserInteraction,
	std::function<void()> &sendChangeUserName
):
	_window(sf::VideoMode({1440, 550}), "Jetpack Joyride", sf::Style::Close | sf::Style::Titlebar),
	_sendUserEvent(sendUserInteraction),
	_sendChangeUserName(sendChangeUserName)

{
	this->_windowType = MENU;
	this->_gameBackgroundTexture = sf::Texture();
	this->_menuBackgroundTexture = sf::Texture();
	this->_blurTexture = sf::Texture();
	if (!this->_gameBackgroundTexture.loadFromFile("src/client/assets/GameBackground.png")) {
		throw std::runtime_error("Erreur : Impossible de charger GameBackground.png");
	}
	if (!this->_menuBackgroundTexture.loadFromFile("src/client/assets/MenuBackground.png")) {
		throw std::runtime_error("Erreur : Impossible de charger MenuBackground.png");
	}
	if (!this->_blurTexture.loadFromFile("src/client/assets/Blur.png")) {
		throw std::runtime_error("Erreur : Impossible de charger Blur.png");
	}
	this->_jetpackFont = sf::Font();
	if (!this->_jetpackFont.loadFromFile("src/client/assets/JetpackFont.ttf")) {
		throw std::runtime_error("Erreur : Impossible de charger la police JetpackFont.ttf");
	}
	this->_menuTextColor = sf::Color(255, 197, 84, 255);
	this->_menuButtonColor = sf::Color(255, 197, 84, 255);
	this->_menuButtonTextColor = sf::Color(219, 218, 214, 255);
	this->_menuUsernameBoxColor	= sf::Color(255, 156, 58, 255);
	this->_menuCountdown = sf::Text();
	this->_usernameTextButton = sf::Text();
	this->_usernameBoxContent = sf::Text();
	this->_usernameButton = sf::RectangleShape();
	this->_usernameBox = sf::RectangleShape();
	this->_menuCountdown.setFont(this->_jetpackFont);
	this->_menuCountdown.setString("2 Player Required");
	this->_menuCountdown.setCharacterSize(40);
	this->_menuCountdown.setFillColor(this->_menuTextColor);
	this->_menuCountdown.setOutlineThickness(3);
	this->_menuCountdown.setOutlineColor(sf::Color::Black);
	this->_menuCountdown.setPosition({1120, 20});
	this->_usernameButton.setPosition({20, 20});
	this->_usernameButton.setFillColor(this->_menuButtonColor);
	this->_usernameButton.setSize({250, 70});
	this->_usernameButton.setOutlineColor(sf::Color::Black);
	this->_usernameButton.setOutlineThickness(3);
	this->_usernameTextButton.setFont(this->_jetpackFont);
	this->_usernameTextButton.setString("Change Username");
	this->_usernameTextButton.setCharacterSize(30);
	this->_usernameTextButton.setFillColor(this->_menuButtonTextColor);
	this->_usernameTextButton.setOutlineThickness(3);
	this->_usernameTextButton.setOutlineColor(sf::Color::Black);
	this->_usernameTextButton.setPosition({36, 35});
	this->_usernameBox.setSize({600, 200});
	this->_usernameBox.setFillColor(this->_menuUsernameBoxColor);
	this->_usernameBox.setPosition({400, 150});
	this->_usernameBox.setOutlineColor(sf::Color::Black);
	this->_usernameBox.setOutlineThickness(4);
	this->_usernameBoxTitle.setFont(this->_jetpackFont);
	this->_usernameBoxTitle.setString("Enter your username");
	this->_usernameBoxTitle.setCharacterSize(40);
	this->_usernameBoxTitle.setFillColor(this->_menuButtonTextColor);
	this->_usernameBoxTitle.setOutlineThickness(3);
	this->_usernameBoxTitle.setOutlineColor(sf::Color::Black);
	this->_usernameBoxTitle.setPosition({440, 175});
	this->_usernameField.setSize({500, 50});
	this->_usernameField.setFillColor(sf::Color::White);
	this->_usernameField.setPosition({440, 250});
	this->_usernameField.setOutlineColor(sf::Color::Black);
	this->_usernameField.setOutlineThickness(3);
	this->_usernameBoxContent.setFont(this->_jetpackFont);
	this->_usernameBoxContent.setString(this->_username);
	this->_usernameBoxContent.setCharacterSize(30);
	this->_usernameBoxContent.setFillColor(this->_menuTextColor);
	this->_usernameBoxContent.setOutlineThickness(3);
	this->_usernameBoxContent.setOutlineColor(sf::Color::Black);
	this->_usernameBoxContent.setPosition({450, 260});

	this->_gameBackground = sf::Sprite(this->_gameBackgroundTexture);
	this->_menuBackground = sf::Sprite(this->_menuBackgroundTexture);
	this->_blurSprite = sf::Sprite(this->_blurTexture);
	this->_window.setFramerateLimit(144);
}
