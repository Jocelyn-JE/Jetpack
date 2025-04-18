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
#include <vector>
#include <EndGame/EndGame.hpp>

#include <SFML/Graphics.hpp>

#include "Coin/CoinGraphic.hpp"
#include "Game/GameGraphic.hpp"
#include "Laser/LaserGraphic.hpp"
#include "Menu/MenuGraphic.hpp"
#include "Player/PlayerGraphic.hpp"
#include "userInteractions.hpp"

namespace jetpack::Client {
class Graphic {
 private:
    enum WindowType { GAME, MENU, TYPE_COUNT };

    WindowType _windowType;
    size_t _gameSpeed = 0;
    bool _isEndGame = false;

    sf::RenderWindow _window;
    std::map<unsigned int, std::pair<std::unique_ptr<Player>, sf::Vector2f> >
        _listPlayers;
    std::vector<sf::Vector2f> _posCoin;

    std::vector<sf::Vector2f> _posLaser;

    std::mutex _posMutex;

    std::function<int()> &_getIdWithAuth;

    Menu _menu;
    Game _game;
    EndGame _endGame;

    std::function<void()> _switchToMenuFct =
    ([this]() {
        this->switchToMenu();
    });

 public:
    void display();

    void clearCoinPos();

    void clearLaserPos();

    bool isOpen() const { return this->_window.isOpen(); }

    void compute();

    void close() { return this->_window.close(); }

    void setPosPlayer(unsigned int id, sf::Vector2f pos);

    void setCoinAmount(unsigned int id, int coins);

    void setPosCoin(sf::Vector2f pos);

    void setPosLaser(sf::Vector2f pos);

    void analyse();

    void setGameSpeed(size_t value);

    void addNewPlayer(unsigned int id, bool isCurrent);

    void setPlayerStatus(unsigned int id, bool isDead);

    void switchToGame();

    void switchToMenu();

    void switchToDeath();

    void serverError();

    void serverOK();

    explicit Graphic(
        std::function<void(UserInteractions_s)> &sendUserInteraction,
        std::function<void(std::string)> &changeUsername,
        std::function<std::string()> &getUsername,
        std::function<std::pair<std::string, std::string>()> &getSocketSettings,
        std::function<void(std::pair<std::string, int>)> &sendSocketSetting,
        std::function<int()> &getIdWithAuth,
        std::function<bool()> &getIsConnectedWithAuth,
        std::function<void()> &sendStartGame);

    ~Graphic() = default;
};
}  // namespace jetpack::Client
#endif  // SRC_CLIENT_GRAPHIC_HPP_
