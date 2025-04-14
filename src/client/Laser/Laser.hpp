//
// Created by roussierenoa on 4/12/25.
//

#ifndef SRC_CLIENT_LASER_LASER_HPP_
#define SRC_CLIENT_LASER_LASER_HPP_
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
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_LASER_LASER_HPP_
