//
// Created by roussierenoa on 4/11/25.
//

#ifndef SRC_CLIENT_GAME_GAMEGRAPHIC_HPP_
#define SRC_CLIENT_GAME_GAMEGRAPHIC_HPP_
#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SpriteSheet.hpp>

#include "../CoinStats/CoinStats.hpp"
#include "../userInteractions.hpp"
#include "Sprite.hpp"
#include "../Coin/CoinGraphic.hpp"
#include "../Laser/LaserGraphic.hpp"

namespace jetpack::Client {
class Game {
 private:
    std::function<void(UserInteractions_s)> &_sendUserInteraction;

    bool _isBg1 = true;
    sf::Clock _clock;

    Sprite _background;
    Sprite _background2;
    CoinStats _coinStats;

    Coin _coin;
    Laser _laser;

    void _handleKeyPressed(const sf::Event &event);

 public:
    void setCoinsAmount(unsigned int coinsAmount);

    void display(sf::RenderWindow &window, std::vector<sf::Vector2f>
        _posCoin, std::vector<sf::Vector2f> _posLaser);

    void compute();

    void analyze(const sf::Event &event);

    explicit Game(std::function<void(UserInteractions_s)> &sendUserInteraction);

    ~Game();
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_GAME_GAMEGRAPHIC_HPP_
