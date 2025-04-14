//
// Created by roussierenoa on 4/11/25.
//

#include "Sprite.hpp"

void Sprite::display(sf::RenderWindow &window) {
    window.draw(this->_sprite);
}

void Sprite::changePos(sf::Vector2f pos) {
    this->_sprite.setPosition(pos);
}

sf::Vector2f Sprite::getPos() {
    return this->_sprite.getPosition();
}

Sprite::Sprite(const std::string &path, sf::Vector2f pos, sf::Vector2f scale) {
    this->_texture = sf::Texture();
    this->_sprite = sf::Sprite();
    if (!this->_texture.loadFromFile(path)) {
        throw std::runtime_error("Erreur : Impossible de charger " + path);
    }
    this->_sprite = sf::Sprite(this->_texture);
    this->_sprite.setPosition(pos);
    this->_sprite.setScale(scale);
}

Sprite::Sprite(const std::string &path) {
    this->_texture = sf::Texture();
    if (!this->_texture.loadFromFile(path)) {
        throw std::runtime_error("Erreur : Impossible de charger " + path);
    }
    this->_sprite = sf::Sprite(this->_texture);
}
