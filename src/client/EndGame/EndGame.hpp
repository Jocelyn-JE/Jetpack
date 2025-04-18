//
// Created by roussierenoa on 4/18/25.
//

#ifndef ENDGAME_HPP
#define ENDGAME_HPP

#include <functional>
#include <memory>
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

        void compute(int id, const std::map<unsigned int, std::pair<std::unique_ptr<Player>, sf::Vector2f>> &listPlayer);

        EndGame(std::function<void()> &switchToMenu);

        ~EndGame();
};
}



#endif //ENDGAME_HPP
