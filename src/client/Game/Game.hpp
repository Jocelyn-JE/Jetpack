//
// Created by roussierenoa on 4/11/25.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <functional>
#include <SpriteSheet.hpp>
#include <SFML/Graphics.hpp>
#include "Sprite.hpp"
#include "../userInteractions.hpp"
#include "../CoinStats/CoinStats.hpp"

namespace jetpack::Client {
class Game {
private:
   std::function<void(UserInteractions_s)> &_sendUserInteraction;

   bool _isBg1 = true;
   sf::Clock _clock;

   Sprite _background;
   Sprite _background2;
   CoinStats _coinStats;

   void _handleKeyPressed(const sf::Event &event);
public:

   void setCoinsAmount(unsigned int coinsAmount);

   void display(sf::RenderWindow &window);

   void compute();

   void analyze(const sf::Event &event);

   Game(std::function<void(UserInteractions_s)> &sendUserInteraction);

   ~Game();
};

} // jetpack

#endif //GAME_HPP
