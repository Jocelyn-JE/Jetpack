//
// Created by roussierenoa on 4/10/25.
//

#ifndef MENU_HPP
#define MENU_HPP
#include <functional>
#include <SFML/Graphics.hpp>

namespace jetpack::Client {

class Menu {
private:
	bool _isUserNamePressed = false;
	std::string _username;

	sf::Texture _menuBackgroundTexture;
	sf::Font _jetpackFont;
	sf::Color _menuTextColor;
	sf::Color _menuButtonColor;
	sf::Color _menuButtonTextColor;
	sf::Color _menuUsernameBoxColor;
	sf::Sprite _menuBackground;
	sf::Shader _blurShader;
	sf::Text _menuCountdown;
	sf::Text _usernameTextButton;
	sf::RectangleShape _usernameButton;
	sf::RectangleShape _usernameField;
	sf::RectangleShape _usernameBox;
	sf::Text _usernameBoxTitle;
	sf::Text _usernameBoxContent;
	sf::Text _serverStateText;
	std::string _serverStateString = "OK";

	void _handleMousePressed(const sf::Event &event);
	std::function<void(std::string)> _changeUsername;

public:
	void setUsername(const std::string &data);

	void setServerStateError();

	void setServerStateOk();

	void display(sf::RenderWindow &window);

	void compute();

	void analyze(const sf::Event &event);

	Menu(std::function<void(std::string)> &changeUsername);

	~Menu() = default;
};
} // jetpack

#endif //MENU_HPP
