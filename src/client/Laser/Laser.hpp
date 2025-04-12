//
// Created by roussierenoa on 4/12/25.
//

#ifndef LASER_HPP
#define LASER_HPP
#include <mutex>
#include <SFML/Graphics.hpp>
#include "SpriteSheet.hpp"

namespace jetpack::Client {
	class Laser {
	private:
		std::mutex _laserMutex;
		sf::Vector2f _pos;

		sf::Clock _clock;
		SpriteSheet _laserSheet;
	public:
		void display(sf::RenderWindow &window);

		void compute();

		void changePosValue(const sf::Vector2f &pos);

		sf::Vector2f getPos() const;

		Laser();
		~Laser() = default;
	};
} // jetpack

#endif //LASER_HPP
