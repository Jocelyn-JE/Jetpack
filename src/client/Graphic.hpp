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

		WindowType _windowType;
		std::string _username;
		sf::RenderWindow _window;
		std::map<unsigned int, std::pair<sf::RectangleShape, sf::Vector2f> > _listPlayers;
		std::mutex _posMutex;
		sf::Texture _gameBackgroundTexture;
		sf::Texture _menuBackgroundTexture;
		sf::Font _jetpackFont;
		sf::Color _menuTextColor;
		sf::Color _menuButtonColor;
		sf::Color _menuButtonTextColor;
		sf::Sprite _gameBackground;
		sf::Sprite _menuBackground;
		sf::Text _menuCountdown;
		sf::Text _usernameTextBox;
		sf::RectangleShape _usernameBox;
		std::function<void(UserInteractions_s)> &_sendUserEvent;

	public:
		void display();

		bool isOpen() const { return this->_window.isOpen(); }

		void compute();

		void close() { return this->_window.close(); }

		void setPosRectangle(unsigned int id, sf::Vector2f pos);

		void setUsername(const std::string &name);

		void handleEvent();

		void addNewPlayer(unsigned int id, bool isCurrent);

		void switchToGame();

		void switchToMenu();

		explicit Graphic(
			std::function<void(UserInteractions_s)> &_sendUserInteraction);

		~Graphic() = default;
	};
} // namespace jetpack::Client

// jetpack

#endif  // SRC_CLIENT_GRAPHIC_HPP_
