//
// Created by roussierenoa on 4/10/25.
//

#ifndef SRC_CLIENT_MENU_MENU_HPP_
#define SRC_CLIENT_MENU_MENU_HPP_
#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

namespace jetpack::Client {

class Menu {
 private:
    bool _isUserNamePressed = false;
    std::string _username;

    sf::Texture _menuBackgroundTexture;
    sf::Font _jetpackFont;
    sf::Color _menuTextColor;
    sf::Color _menuButtonColor;
    sf::Color _menuButtonTextColor;
    sf::Color _menuUsernameBoxColor;
    sf::Sprite _menuBackground;
    sf::Shader _blurShader;
    sf::Text _menuCountdown;
    sf::Text _menuIDText;
    sf::Text _usernameTextButton;
    sf::RectangleShape _usernameButton;
    sf::RectangleShape _usernameField;
    sf::RectangleShape _usernameBox;
    sf::Text _usernameBoxTitle;
    sf::Text _usernameBoxContent;
    sf::Text _serverStateText;
    std::string _serverStateString = "OK";

    void _handleMousePressed(const sf::Event &event);

    std::function<void(std::string)> &_changeUsername;
    std::function<std::string()> &_getUsername;
    std::function<bool()> &_authIsConnected;
    std::function<int()> &_authGetId;

 public:
    void setServerStateError();

    void setServerStateOk();

    void display(sf::RenderWindow &window);

    void compute();

    void analyze(const sf::Event &event);

    Menu(std::function<void(std::string)> &changeUsername,
        std::function<std::string()> &getUsername,
        std::function<int()> &getIdWithAuth,
        std::function<bool()> &getIsConnectedWithAuth);

    ~Menu() = default;
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_MENU_MENU_HPP_
