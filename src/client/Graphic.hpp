//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_GRAPHIC_HPP_
#define SRC_CLIENT_GRAPHIC_HPP_

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Coin/Coin.hpp"
#include "Game/Game.hpp"
#include "Laser/Laser.hpp"
#include "Menu/Menu.hpp"
#include "Player/Player.hpp"
#include "userInteractions.hpp"

namespace jetpack::Client {
class Graphic {
 private:
    enum WindowType { GAME, MENU, TYPE_COUNT };

    WindowType _windowType;
    sf::RenderWindow _window;
    std::map<unsigned int, std::pair<std::unique_ptr<Player>, sf::Vector2f> >
        _listPlayers;
    std::map<unsigned int, std::pair<std::unique_ptr<Coin>, sf::Vector2f> >
        _listCoins;
    std::map<unsigned int, std::pair<std::unique_ptr<Laser>, sf::Vector2f> >
        _listLasers;
    std::mutex _posMutex;
    std::function<void(UserInteractions_s)> &_sendUserEvent;
    std::function<void(std::string)> &_changeUsername;
    std::function<std::string()> &_getUsername;

    Menu _menu;
    Game _game;

 public:
    void display();

    bool isOpen() const { return this->_window.isOpen(); }

    void compute();

    void close() { return this->_window.close(); }

    void setPosPlayer(unsigned int id, sf::Vector2f pos);

    void setPosCoin(unsigned int id, sf::Vector2f pos);

    void setPosLaser(unsigned int id, sf::Vector2f pos);

    void analyse();

    void addNewPlayer(unsigned int id, bool isCurrent);

    void addNewCoin(unsigned int id);

    void addNewLaser(unsigned int id);

    void switchToGame();

    void switchToMenu();

    void serverError();

    void serverOK();

    explicit Graphic(
        std::function<void(UserInteractions_s)> &sendUserInteraction,
        std::function<void(std::string)> &changeUsername,
        std::function<std::string()> &getUsername);

    ~Graphic() = default;
};
}  // namespace jetpack::Client
#endif  // SRC_CLIENT_GRAPHIC_HPP_
