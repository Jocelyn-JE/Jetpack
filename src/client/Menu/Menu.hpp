//
// Created by roussierenoa on 4/10/25.
//

#ifndef SRC_CLIENT_MENU_MENU_HPP_
#define SRC_CLIENT_MENU_MENU_HPP_
#include <functional>
#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

namespace jetpack::Client {

class Menu {
 private:
    bool _isUserNamePressed = false;
    bool _isSettingsPressed = false;
    bool _isIpSelected = false;
    bool _isPortSelected = false;
    std::string _username;
    std::string _ip;
    std::string _port;

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

    sf::Text _settingsTextButton;
    sf::RectangleShape _settingsButton;
    sf::RectangleShape _ipField;
    sf::RectangleShape _portField;
    sf::RectangleShape _settingsBox;
    sf::Text _settingsBoxTitle;
    sf::Text _ipBoxContent;
    sf::Text _ipBoxTitle;
    sf::Text _portBoxContent;
    sf::Text _portBoxTitle;

    sf::Text _serverStateText;
    std::string _serverStateString = "OK";

    void _handleMousePressed(const sf::Event &event);

    std::function<void(std::string)> &_changeUsername;
    std::function<std::string()> &_getUsername;
    std::function<bool()> &_authIsConnected;
    std::function<int()> &_authGetId;
    std::function<std::pair<std::string, std::string>()> &_getSocketSettings;
    std::function<void(std::pair<std::string, int>)> &_sendSocketSettings;

 public:
    void setServerStateError();

    void setServerStateOk();

    void display(sf::RenderWindow &window);

    void compute();

    void analyze(const sf::Event &event);

    Menu(
        std::function<void(std::string)> &changeUsername,
        std::function<std::string()> &getUsername,
        std::function<std::pair<std::string, std::string>()> &getSocketSettings,
        std::function<void(std::pair<std::string, int>)> &sendSocketSettings,
        std::function<int()> &getIdWithAuth,
        std::function<bool()> &getIsConnectedWithAuth);

    ~Menu() = default;
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_MENU_MENU_HPP_
