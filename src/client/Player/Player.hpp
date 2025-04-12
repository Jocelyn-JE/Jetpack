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
   sf::Vector2f _pos;
   bool _visibility = false;
   std::string _username;
   std::mutex _playerMutex;
   sf::Clock _clock;

   SpriteSheet _player;
public:
   void display(sf::RenderWindow &window);

   void compute();

   void changePosValue(sf::Vector2f pos);

   Player(bool isPlayer);
   ~Player();
};

} // jetpack

#endif //PLAYER_HPP
