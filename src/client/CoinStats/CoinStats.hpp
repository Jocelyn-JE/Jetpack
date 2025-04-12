//
// Created by roussierenoa on 4/12/25.
//

#ifndef COINSTATS_HPP
#define COINSTATS_HPP
#include <iostream>
#include <SFML/Graphics.hpp>
#include "SpriteSheet.hpp"

namespace jetpack::Client {
	class CoinStats {
	private:
		unsigned int _coinsAmount = 0;
		sf::Font _jetpackFont;
		sf::Text _coinAmountText;
		SpriteSheet _coinSprite;
	public:
		void setCoinsAmount(unsigned int coinsAmount);
		void display(sf::RenderWindow &window);
		void compute();
		CoinStats();
		~CoinStats() = default;
	};
} // jetpack
#endif //COINSTATS_HPP