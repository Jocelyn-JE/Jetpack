//
// Created by roussierenoa on 4/11/25.
//

#include "SpriteSheet.hpp"

void SpriteSheet::display(sf::RenderWindow &window) {
	window.draw(this->_sprite);
}

void SpriteSheet::setSprite(int col, int line) {
	if (col >= this->_nbrColumns || line >= this->_nbrLines)
		throw std::runtime_error("Erreur : Impossible de changer le sprite");
	this->_rect.left = col * this->_sizeElements.x;
	this->_rect.top = line * this->_sizeElements.y;
}

void SpriteSheet::setPos(sf::Vector2f pos) {
	this->_sprite.setPosition(pos);
}

void SpriteSheet::setTransparency() {
	sf::Color color = this->_sprite.getColor();
	color.a = 180;
	this->_sprite.setColor(color);
}

SpriteSheet::SpriteSheet(const std::string &path, sf::Vector2i sizeElements, int nbrLines, int nbrColumns, sf::Vector2f scale
) :
	_sizeElements(sizeElements),
	_nbrLines(nbrLines),
	_nbrColumns(nbrColumns)
{
	this->_sizeElements = sizeElements;
	this->_texture = sf::Texture();
	if (!this->_texture.loadFromFile(path))
		throw std::runtime_error("Erreur : Impossible de charger " + path);
	this->_rect.width = this->_sizeElements.x;
	this->_rect.height = this->_sizeElements.y;
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setTextureRect(this->_rect);
	this->_sprite.setScale(scale);
}
