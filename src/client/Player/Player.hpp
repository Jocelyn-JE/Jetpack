//
// Created by roussierenoa on 4/11/25.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <mutex>

#include "SpriteSheet.hpp"
#include "Sprite.hpp"

namespace jetpack::Client {
class Player {
   enum PlayerSprite_t {
      RUNNING = 0,
      JETPACK_ON = 1,
      LANDING = 2,
      ELECTRIC_SHOCK = 3,
      FIRE_SHOCK = 4,
      SHOCK = 5,
   };
   PlayerSprite_t _currentSpriteState = RUNNING;
   bool _isHost = false;
   unsigned int _coinsAmount = 182;
   sf::Vector2f _pos;
   std::string _username;
   std::mutex _playerMutex;
   std::mutex _coinMutex;
   sf::Clock _clock;

   SpriteSheet _player;
public:

   unsigned int getCoinsAmount();

   void setCoinsAmount(unsigned int coins);

   bool isHost() const {;
      return this->_isHost;
   };

   void display(sf::RenderWindow &window);

   void compute();

   void changePosValue(sf::Vector2f pos);

   Player(bool isPlayer);

   ~Player();
};

} // jetpack

#endif //PLAYER_HPP
