//
// Created by roussierenoa on 4/18/25.
//

#ifndef SRC_CLIENT_ENDGAME_ENDGAME_HPP_
#define SRC_CLIENT_ENDGAME_ENDGAME_HPP_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Player/PlayerGraphic.hpp"

namespace jetpack::Client {
class EndGame {
 private:
    sf::Clock _endGameClock;
    sf::Text _text;
    sf::Font _font;
    sf::RenderWindow _window;

    std::function<void()> &_switchToMenu;

 public:
    void resetEndGameClock();

    void display(sf::RenderWindow &window);

    void compute(
        int id, const std::map<unsigned int,
                               std::pair<std::unique_ptr<Player>, sf::Vector2f>>
                    &listPlayer);

    explicit EndGame(std::function<void()> &switchToMenu);

    ~EndGame();
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_ENDGAME_ENDGAME_HPP_
