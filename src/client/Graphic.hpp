//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_GRAPHIC_HPP_
#define SRC_CLIENT_GRAPHIC_HPP_

#include <functional>
#include <map>
#include <mutex>
#include <utility>

#include <SFML/Graphics.hpp>

#include "userInteractions.hpp"

namespace jetpack::Client {
	class Graphic {
	private:
		enum WindowType {
			GAME,
			MENU,
			TYPE_COUNT
		};
		bool _isUserNamePressed = false;

		WindowType _windowType;
		std::string _username;
		sf::RenderWindow _window;
		std::map<unsigned int, std::pair<sf::RectangleShape, sf::Vector2f> > _listPlayers;
		std::mutex _posMutex;
		sf::Texture _gameBackgroundTexture;
		sf::Texture _menuBackgroundTexture;
		sf::Texture _blurTexture;
		sf::Font _jetpackFont;
		sf::Color _menuTextColor;
		sf::Color _menuButtonColor;
		sf::Color _menuButtonTextColor;
		sf::Color _menuUsernameBoxColor;
		sf::Sprite _gameBackground;
		sf::Sprite _menuBackground;
		sf::Sprite _blurSprite;
		sf::Text _menuCountdown;
		sf::Text _usernameTextButton;
		sf::RectangleShape _usernameButton;
		sf::RectangleShape _usernameField;
		sf::RectangleShape _usernameBox;
		sf::Text _usernameBoxTitle;
		sf::Text _usernameBoxContent;
		std::function<void(UserInteractions_s)> &_sendUserEvent;
		std::function<void()> _sendChangeUserName;

		void _handleKeyPressed(const sf::Event &);
		void _handleMousePressed(const sf::Event &);

	public:
		void display();

		bool isOpen() const { return this->_window.isOpen(); }

		void compute();

		void close() { return this->_window.close(); }

		void setPosRectangle(unsigned int id, sf::Vector2f pos);

		void setUsername(const std::string &name);

		std::string getUsername() const {
			return this->_username;
		}

		void analyse();

		void addNewPlayer(unsigned int id, bool isCurrent);

		void switchToGame();

		void switchToMenu();

		explicit Graphic(
			std::function<void(UserInteractions_s)> &sendUserInteraction,
			std::function<void()> &sendChangeUserName
		);

		~Graphic() = default;
	};
} // namespace jetpack::Client

// jetpack

#endif  // SRC_CLIENT_GRAPHIC_HPP_
