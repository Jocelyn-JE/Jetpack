//
// Created by roussierenoa on 4/12/25.
//

#ifndef SRC_CLIENT_COIN_COIN_HPP_
#define SRC_CLIENT_COIN_COIN_HPP_
#include <mutex>

#include <SFML/Graphics.hpp>
#include <SpriteSheet.hpp>

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
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_COIN_COIN_HPP_
