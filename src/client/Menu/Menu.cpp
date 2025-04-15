//
// Created by roussierenoa on 4/10/25.
//

#include <string>
#include <utility>
#include "Menu.hpp"


void jetpack::Client::Menu::_handleMousePressed(const sf::Event &event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        if (!this->_isSettingsPressed &&
            this->_usernameButton.getGlobalBounds().contains(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y))) {
            this->_isUserNamePressed = !this->_isUserNamePressed;
            if (this->_isUserNamePressed) {
                this->_usernameTextButton.setString("Close");
                this->_usernameButton.setSize({105, 70});
                this->_settingsButton.setFillColor(sf::Color(71, 71, 70));
            } else {
                if (!this->_username.empty() &&
                    this->_getUsername() != this->_username)
                    this->_changeUsername(this->_username);
                this->_usernameTextButton.setString("Change Username");
                this->_usernameButton.setSize({250, 70});
                this->_settingsButton.setFillColor(this->_menuButtonColor);
            }
        }
    }
}

void jetpack::Client::Menu::display(sf::RenderWindow &window) {
    if (!this->_isUserNamePressed && !this->_isSettingsPressed)
        window.draw(this->_menuBackground);
    else
        window.draw(this->_menuBackground, &this->_blurShader);
    window.draw(this->_menuCountdown);
    window.draw(this->_menuIDText);
    window.draw(this->_serverStateText);
    window.draw(this->_usernameButton);
    window.draw(this->_usernameTextButton);
    window.draw(this->_settingsButton);
    window.draw(this->_settingsTextButton);
    if (this->_isUserNamePressed) {
        window.draw(this->_usernameBox);
        window.draw(this->_usernameBoxTitle);
        window.draw(this->_usernameField);
        window.draw(this->_usernameBoxContent);
    }
    if (this->_isSettingsPressed) {
        window.draw(this->_settingsBox);
        window.draw(this->_settingsBoxTitle);
        window.draw(this->_ipField);
        window.draw(this->_ipBoxContent);
        window.draw(this->_ipBoxTitle);
        window.draw(this->_portField);
        window.draw(this->_portBoxContent);
        window.draw(this->_portBoxTitle);
    }
}

void jetpack::Client::Menu::compute() {
    if (this->_authIsConnected()) {
        this->_menuIDText.setFillColor(this->_menuTextColor);
        this->_menuIDText.setString("ID: " +
            std::to_string(this->_authGetId()));
    } else {
        this->_menuIDText.setFillColor(sf::Color::Red);
        this->_menuIDText.setString("ID: none");
    }
    if (this->_isIpSelected)
        this->_ipField.setOutlineColor(sf::Color::Green);
    else
        this->_ipField.setOutlineColor(sf::Color::Black);
    if (this->_isPortSelected)
        this->_portField.setOutlineColor(sf::Color::Green);
    else
        this->_portField.setOutlineColor(sf::Color::Black);
    if (!this->_isSettingsPressed) {
        if (this->_ip != this->_getSocketSettings().first) {
            this->_ip = this->_getSocketSettings().first;
            this->_ipBoxContent.setString(this->_getSocketSettings().first);
        }
        if (this->_port != this->_getSocketSettings().second) {
            this->_port = this->_getSocketSettings().second;
            this->_portBoxContent.setString(this->_getSocketSettings().second);
        }
    }
}

void jetpack::Client::Menu::analyze(const sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed)
        this->_handleMousePressed(event);
    if (event.type == sf::Event::TextEntered && this->_isUserNamePressed) {
        if (event.text.unicode >= 32 && event.text.unicode <= 126 &&
            this->_username.length() < 20)
            this->_username += static_cast<char>(event.text.unicode);
        else if (event.text.unicode == 8 && !this->_username.empty())
            this->_username.pop_back();
        this->_usernameBoxContent.setString(this->_username);
    }
    if (event.type == sf::Event::TextEntered && this->_isIpSelected) {
        if (((event.text.unicode >= '0' && event.text.unicode <= '9')
                || event.text.unicode == '.'))
            this->_ip += static_cast<char>(event.text.unicode);
        else if (event.text.unicode == 8 && !this->_ip.empty())
            this->_ip.pop_back();
        this->_ipBoxContent.setString(this->_ip);
    }
    if (event.type == sf::Event::TextEntered && this->_isPortSelected) {
        if (event.text.unicode >= '0' && event.text.unicode <= '9')
            this->_port += static_cast<char>(event.text.unicode);
        else if (event.text.unicode == 8 && !this->_port.empty())
            this->_port.pop_back();
        this->_portBoxContent.setString(this->_port);
    }
}

void jetpack::Client::Menu::setServerStateError() {
    this->_serverStateString = "KO";
    this->_serverStateText.setFillColor(sf::Color::Red);
    this->_serverStateText.setOutlineColor(sf::Color::Black);
    this->_serverStateText.setOutlineThickness(3);
    this->_serverStateText.setString("Server: " + this->_serverStateString);
}

void jetpack::Client::Menu::setServerStateOk() {
    this->_serverStateString = "OK";
    this->_serverStateText.setFillColor(this->_menuTextColor);
    this->_serverStateText.setOutlineColor(sf::Color::Black);
    this->_serverStateText.setOutlineThickness(3);
    this->_serverStateText.setString("Server: " + this->_serverStateString);
}

jetpack::Client::Menu::Menu(std::function<void(std::string)> &changeUsername,
    std::function<std::string()> &getUsername,
    std::function<std::pair<std::string, std::string>()> &getSocketSettings,
    std::function<void(std::pair<std::string, int>)> &sendSocketSettings,
    std::function<int()> &getIdWithAuth,
    std::function<bool()> &getIsConnectedWithAuth
):
    _changeUsername(changeUsername),
    _getUsername(getUsername),
    _authIsConnected(getIsConnectedWithAuth),
    _authGetId(getIdWithAuth),
    _getSocketSettings(getSocketSettings),
    _sendSocketSettings(sendSocketSettings) {
    if (!this->_menuBackgroundTexture.loadFromFile("src/client/assets/"
                                                   "MenuBackground.png")) {
        throw std::runtime_error(
            "Erreur : Impossible de charger MenuBackground.png");
    }
    if (!this->_blurShader.loadFromFile("src/client/assets/blur.frag",
                                        sf::Shader::Fragment)) {
        throw std::runtime_error(
            "Erreur : Impossible de charger"
            " le shader blur.frag");
    }
    if (!this->_jetpackFont.loadFromFile("src/client/assets/"
                                         "JetpackFont.ttf")) {
        throw std::runtime_error(
            "Erreur : Impossible de charger la police"
            " JetpackFont.ttf");
    }
    this->_menuTextColor = sf::Color(255, 197, 84, 255);
    this->_menuButtonColor = sf::Color(255, 197, 84, 255);
    this->_menuButtonTextColor = sf::Color(219, 218, 214, 255);
    this->_menuUsernameBoxColor = sf::Color(255, 156, 58, 255);

    this->_menuCountdown.setFont(this->_jetpackFont);
    this->_menuCountdown.setString("2 Player Required");
    this->_menuCountdown.setCharacterSize(40);
    this->_menuCountdown.setFillColor(this->_menuTextColor);
    this->_menuCountdown.setOutlineThickness(3);
    this->_menuCountdown.setOutlineColor(sf::Color::Black);
    this->_menuCountdown.setPosition({1120, 20});

    this->_menuIDText.setFont(this->_jetpackFont);
    this->_menuIDText.setString("ID: none");
    this->_menuIDText.setCharacterSize(40);
    this->_menuIDText.setFillColor(sf::Color::Red);
    this->_menuIDText.setOutlineThickness(3);
    this->_menuIDText.setOutlineColor(sf::Color::Black);
    this->_menuIDText.setPosition({1300, 120});

    this->_serverStateText.setFont(this->_jetpackFont);
    this->_serverStateText.setString("Server: " + this->_serverStateString);
    this->_serverStateText.setCharacterSize(40);
    this->_serverStateText.setFillColor(this->_menuTextColor);
    this->_serverStateText.setOutlineThickness(3);
    this->_serverStateText.setOutlineColor(sf::Color::Black);
    this->_serverStateText.setPosition({1250, 70});

    this->_usernameButton.setPosition({20, 20});
    this->_usernameButton.setFillColor(this->_menuButtonColor);
    this->_usernameButton.setSize({250, 70});
    this->_usernameButton.setOutlineColor(sf::Color::Black);
    this->_usernameButton.setOutlineThickness(3);

    this->_usernameTextButton.setFont(this->_jetpackFont);
    this->_usernameTextButton.setString("Change Username");
    this->_usernameTextButton.setCharacterSize(30);
    this->_usernameTextButton.setFillColor(this->_menuButtonTextColor);
    this->_usernameTextButton.setOutlineThickness(3);
    this->_usernameTextButton.setOutlineColor(sf::Color::Black);
    this->_usernameTextButton.setPosition({36, 35});

    this->_usernameBox.setSize({600, 200});
    this->_usernameBox.setFillColor(this->_menuUsernameBoxColor);
    this->_usernameBox.setPosition({400, 150});
    this->_usernameBox.setOutlineColor(sf::Color::Black);
    this->_usernameBox.setOutlineThickness(4);

    this->_usernameBoxTitle.setFont(this->_jetpackFont);
    this->_usernameBoxTitle.setString("Enter your username");
    this->_usernameBoxTitle.setCharacterSize(40);
    this->_usernameBoxTitle.setFillColor(this->_menuButtonTextColor);
    this->_usernameBoxTitle.setOutlineThickness(3);
    this->_usernameBoxTitle.setOutlineColor(sf::Color::Black);
    this->_usernameBoxTitle.setPosition({440, 175});

    this->_usernameField.setSize({500, 50});
    this->_usernameField.setFillColor(sf::Color::White);
    this->_usernameField.setPosition({440, 250});
    this->_usernameField.setOutlineColor(sf::Color::Black);
    this->_usernameField.setOutlineThickness(3);

    this->_usernameBoxContent.setFont(this->_jetpackFont);
    this->_usernameBoxContent.setString(this->_username);
    this->_usernameBoxContent.setCharacterSize(30);
    this->_usernameBoxContent.setFillColor(this->_menuTextColor);
    this->_usernameBoxContent.setOutlineThickness(3);
    this->_usernameBoxContent.setOutlineColor(sf::Color::Black);
    this->_usernameBoxContent.setPosition({450, 260});

    this->_settingsButton.setPosition({20, 100});
    this->_settingsButton.setFillColor(this->_menuButtonColor);
    this->_settingsButton.setSize({140, 70});
    this->_settingsButton.setOutlineColor(sf::Color::Black);
    this->_settingsButton.setOutlineThickness(3);

    this->_settingsTextButton.setFont(this->_jetpackFont);
    this->_settingsTextButton.setString("Settings");
    this->_settingsTextButton.setCharacterSize(30);
    this->_settingsTextButton.setFillColor(this->_menuButtonTextColor);
    this->_settingsTextButton.setOutlineThickness(3);
    this->_settingsTextButton.setOutlineColor(sf::Color::Black);
    this->_settingsTextButton.setPosition({36, 115});

    this->_settingsBox.setSize({600, 450});
    this->_settingsBox.setFillColor(this->_menuUsernameBoxColor);
    this->_settingsBox.setPosition({400, 50});
    this->_settingsBox.setOutlineColor(sf::Color::Black);
    this->_settingsBox.setOutlineThickness(4);

    this->_settingsBoxTitle.setFont(this->_jetpackFont);
    this->_settingsBoxTitle.setString("Settings");
    this->_settingsBoxTitle.setCharacterSize(40);
    this->_settingsBoxTitle.setFillColor(this->_menuButtonTextColor);
    this->_settingsBoxTitle.setOutlineThickness(3);
    this->_settingsBoxTitle.setOutlineColor(sf::Color::Black);
    this->_settingsBoxTitle.setPosition({645, 50});

    this->_ipBoxTitle.setFont(this->_jetpackFont);
    this->_ipBoxTitle.setString("IP Address");
    this->_ipBoxTitle.setCharacterSize(30);
    this->_ipBoxTitle.setFillColor(this->_menuButtonTextColor);
    this->_ipBoxTitle.setOutlineThickness(3);
    this->_ipBoxTitle.setOutlineColor(sf::Color::Black);
    this->_ipBoxTitle.setPosition({440, 150});

    this->_ipField.setSize({500, 50});
    this->_ipField.setFillColor(sf::Color::White);
    this->_ipField.setPosition({440, 210});
    this->_ipField.setOutlineColor(sf::Color::Black);
    this->_ipField.setOutlineThickness(3);

    this->_ipBoxContent.setFont(this->_jetpackFont);
    this->_ipBoxContent.setString(this->_username);
    this->_ipBoxContent.setCharacterSize(30);
    this->_ipBoxContent.setFillColor(this->_menuTextColor);
    this->_ipBoxContent.setOutlineThickness(3);
    this->_ipBoxContent.setOutlineColor(sf::Color::Black);
    this->_ipBoxContent.setPosition({450, 215});

    this->_portBoxTitle.setFont(this->_jetpackFont);
    this->_portBoxTitle.setString("Port");
    this->_portBoxTitle.setCharacterSize(30);
    this->_portBoxTitle.setFillColor(this->_menuButtonTextColor);
    this->_portBoxTitle.setOutlineThickness(3);
    this->_portBoxTitle.setOutlineColor(sf::Color::Black);
    this->_portBoxTitle.setPosition({440, 300});

    this->_portField.setSize({500, 50});
    this->_portField.setFillColor(sf::Color::White);
    this->_portField.setPosition({440, 360});
    this->_portField.setOutlineColor(sf::Color::Black);
    this->_portField.setOutlineThickness(3);

    this->_portBoxContent.setFont(this->_jetpackFont);
    this->_portBoxContent.setString(this->_username);
    this->_portBoxContent.setCharacterSize(30);
    this->_portBoxContent.setFillColor(this->_menuTextColor);
    this->_portBoxContent.setOutlineThickness(3);
    this->_portBoxContent.setOutlineColor(sf::Color::Black);
    this->_portBoxContent.setPosition({450, 365});

    this->_blurShader.setUniform("texture", sf::Shader::CurrentTexture);
    this->_blurShader.setUniform("direction", sf::Vector2f(1.f, 0.f));
    this->_blurShader.setUniform("radius", 10.f);
    this->_blurShader.setUniform("sigma", 5.f);
    this->_blurShader.setUniform("direction", sf::Vector2f(0.f, 1.f));
    this->_blurShader.setUniform("resolution", sf::Vector2f(1440.f, 550.f));

    this->_menuBackground = sf::Sprite(this->_menuBackgroundTexture);
}
