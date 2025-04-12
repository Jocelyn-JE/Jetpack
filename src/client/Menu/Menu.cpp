//
// Created by roussierenoa on 4/10/25.
//

#include "Menu.hpp"

namespace jetpack::Client {
	void Menu::_handleMousePressed(const sf::Event &event) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (this->_usernameButton.getGlobalBounds().contains(
				static_cast<float>(event.mouseButton.x),
				static_cast<float>(event.mouseButton.y))) {
				this->_isUserNamePressed = !this->_isUserNamePressed;
				if (this->_isUserNamePressed) {
					this->_usernameTextButton.setString("Close");
					this->_usernameButton.setSize({105, 70});
				} else {
					if (!this->_username.empty() && this->_getUsername() != this->_username)
						this->_changeUsername(this->_username);
					this->_usernameTextButton.setString("Change Username");
					this->_usernameButton.setSize({250, 70});
				}
			}
		}
	}

	void Menu::display(sf::RenderWindow &window) {
		if (!this->_isUserNamePressed)
			window.draw(this->_menuBackground);
		else
			window.draw(this->_menuBackground, &this->_blurShader);
		window.draw(this->_menuCountdown);
		window.draw(this->_serverStateText);
		window.draw(this->_usernameButton);
		window.draw(this->_usernameTextButton);
		if (this->_isUserNamePressed) {
			window.draw(this->_usernameBox);
			window.draw(this->_usernameBoxTitle);
			window.draw(this->_usernameField);
			window.draw(this->_usernameBoxContent);
		}
	}

	void Menu::compute() {
	}

	void Menu::analyze(const sf::Event &event) {
		if (event.type == sf::Event::MouseButtonPressed)
			this->_handleMousePressed(event);
		if (event.type == sf::Event::TextEntered && this->_isUserNamePressed) {
			if (event.text.unicode >= 32 && event.text.unicode <= 126 && this->_username.length() < 20)
				this->_username += static_cast<char>(event.text.unicode);
			else if (event.text.unicode == 8 && !this->_username.empty())
				this->_username.pop_back();
			this->_usernameBoxContent.setString(this->_username);
		}
	}

    void Menu::setServerStateError() {
		this->_serverStateString = "KO";
		this->_serverStateText.setFillColor(sf::Color::Red);
		this->_serverStateText.setOutlineColor(sf::Color::Black);
		this->_serverStateText.setOutlineThickness(3);
		this->_serverStateText.setString("Server: " + this->_serverStateString);
    }

    void Menu::setServerStateOk() {
		this->_serverStateString = "OK";
		this->_serverStateText.setFillColor(this->_menuTextColor);
		this->_serverStateText.setOutlineColor(sf::Color::Black);
		this->_serverStateText.setOutlineThickness(3);
		this->_serverStateText.setString("Server: " + this->_serverStateString);
    }

	Menu::Menu(
		std::function<void(std::string)> &changeUsername,
		std::function<std::string()> &getUsername
	):
		_changeUsername(changeUsername),
		_getUsername(getUsername)
	{
		this->_menuBackgroundTexture = sf::Texture();
		this->_jetpackFont = sf::Font();
		if (!this->_menuBackgroundTexture.loadFromFile("src/client/assets/MenuBackground.png")) {
			throw std::runtime_error("Erreur : Impossible de charger MenuBackground.png");
		}
		if (!this->_blurShader.loadFromFile("src/client/assets/blur.frag", sf::Shader::Fragment)) {
			throw std::runtime_error("Erreur : Impossible de charger le shader blur.frag");
		}
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
		this->_serverStateText = sf::Text();
		this->_usernameButton = sf::RectangleShape();
		this->_usernameBox = sf::RectangleShape();

		this->_menuCountdown.setFont(this->_jetpackFont);
		this->_menuCountdown.setString("2 Player Required");
		this->_menuCountdown.setCharacterSize(40);
		this->_menuCountdown.setFillColor(this->_menuTextColor);
		this->_menuCountdown.setOutlineThickness(3);
		this->_menuCountdown.setOutlineColor(sf::Color::Black);
		this->_menuCountdown.setPosition({1120, 20});

		this->_serverStateText.setFont(this->_jetpackFont);
		this->_serverStateText.setString("Server: " + this->_serverStateString);
		this->_serverStateText.setCharacterSize(40);
		this->_serverStateText.setFillColor(this->_menuTextColor);
		this->_serverStateText.setOutlineThickness(3);
		this->_serverStateText.setOutlineColor(sf::Color::Black);
		this->_serverStateText.setPosition({1250, 70});

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

		this->_blurShader.setUniform("texture", sf::Shader::CurrentTexture);
		this->_blurShader.setUniform("direction", sf::Vector2f(1.f, 0.f));
		this->_blurShader.setUniform("radius", 10.f);
		this->_blurShader.setUniform("sigma", 5.f);
		this->_blurShader.setUniform("direction", sf::Vector2f(0.f, 1.f));
		this->_blurShader.setUniform("resolution", sf::Vector2f(1440.f, 550.f));

		this->_menuBackground = sf::Sprite(this->_menuBackgroundTexture);
	}

} // jetpack