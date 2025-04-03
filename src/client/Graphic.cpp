//
// Created by roussierenoa on 3/28/25.
//

#include "Graphic.hpp"

#include "Program.hpp"

void jetpack::Client::Graphic::display() {
    this->_window.clear();
    if (this->_windowType == MENU) {
        this->_window.draw(this->_gameBackground);
        // TODO(noa) : add menu
    }
    if (this->_windowType == GAME) {
        this->_window.draw(this->_gameBackground);
        for (auto &s : this->_listPlayers) {
            this->_window.draw(s.second.first);
        }
    }
    this->_window.display();
}

void jetpack::Client::Graphic::compute() {
    this->_posMutex.lock();
    if (this->_windowType == MENU) {
        // TODO(noa) : add menu
    }
    if (this->_windowType == GAME) {
        for (auto &s : this->_listPlayers) {
            s.second.first.setPosition(s.second.second);
        }
    }
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::setPosRectangle(unsigned int id,
                                               sf::Vector2f pos) {
    this->_posMutex.lock();
    for (auto s : this->_listPlayers) this->_listPlayers.at(id).second = pos;
    this->_posMutex.unlock();
}

void jetpack::Client::Graphic::handleEvent() {
    sf::Event event{};
    while (this->_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) this->_window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                this->_sendUserEvent(UserInteractions_s::UP);
            }
            // TODO(noa) : demander les autres interaction possible entre le
            // server et le client
            if (event.key.code == sf::Keyboard::Escape) {
                this->_sendUserEvent(UserInteractions_s::ESCAPE);
            }
            //
        }
    }
}

void jetpack::Client::Graphic::addNewPlayer(unsigned int id, bool isCurrent) {
    (void)isCurrent;
    // Is current == is the player that is playing
    if (!this->_listPlayers.contains(id)) {
        sf::RectangleShape rect;
        rect.setSize({50.0f, 50.0f});
        rect.setFillColor(sf::Color::Red);
        sf::Vector2f pos = rect.getPosition();
        this->_listPlayers.insert({id, {rect, pos}});
    }
}

jetpack::Client::Graphic::Graphic(
    std::function<void(UserInteractions_s)> &sendUserInteraction)
    : _window(sf::VideoMode({400, 350}), "jetpack",
              sf::Style::Close | sf::Style::Titlebar),
      _sendUserEvent(sendUserInteraction) {
    this->_windowType = MENU;
    this->_gameBackgroundTexture = sf::Texture();
    this->_gameBackgroundTexture.loadFromFile("src/client/assets/background.png");
    this->_gameBackground = sf::Sprite(this->_gameBackgroundTexture);

    this->_window.setFramerateLimit(144);
}
