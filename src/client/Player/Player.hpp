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
   sf::Vector2f _pos;
   bool _visibility = false;
   std::string _username;
   std::mutex _playerMutex;

   SpriteSheet _player;
public:
   void display(sf::RenderWindow &window);

   void compute();

   void changePosValue(sf::Vector2f pos);

   Player(bool isPlayer = false);
   ~Player();
};

} // jetpack

#endif //PLAYER_HPP
