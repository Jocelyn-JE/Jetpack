//
// Created by roussierenoa on 4/12/25.
//

#ifndef COIN_HPP
#define COIN_HPP
#include <mutex>
#include <SpriteSheet.hpp>
#include <SFML/Graphics.hpp>

namespace jetpack::Client {
	class Coin {
	private:
		std::mutex _coinMutex;
		sf::Vector2f _pos;

		sf::Clock _clock;
		SpriteSheet _coinSheet;
	public:
		void display(sf::RenderWindow &window);

		void compute();

		void changePosValue(const sf::Vector2f &pos);

		sf::Vector2f getPos() const;

		Coin();
		~Coin() = default;
	};
} // jetpack

#endif //COIN_HPP
