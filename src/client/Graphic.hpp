//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_GRAPHIC_HPP_
#define SRC_CLIENT_GRAPHIC_HPP_

#include <functional>
#include <map>
#include <mutex>
#include <utility>

#include <SFML/Graphics.hpp>

#include "userInteractions.hpp"

namespace jetpack::Client {
class Graphic {
 private:
    sf::RenderWindow _window;
    std::map<unsigned int, std::pair<sf::RectangleShape, sf::Vector2f>>
        _listPlayers;
    std::mutex _posMutex;
    std::function<void(UserInteractions_s)> &_sendUserEvent;

 public:
    void display();
    bool isOpen() const { return this->_window.isOpen(); }
    void compute();
    void close() { return this->_window.close(); }
    void setPosRectangle(unsigned int id, sf::Vector2f pos);
    void handleEvent();
    void addNewPlayer(unsigned int id, bool isCurrent);

    explicit Graphic(
        std::function<void(UserInteractions_s)> &_sendUserInteraction);
    ~Graphic() = default;
};
}  // namespace jetpack::Client

// jetpack

#endif  // SRC_CLIENT_GRAPHIC_HPP_
