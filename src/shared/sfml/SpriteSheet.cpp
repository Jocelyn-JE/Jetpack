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
	this->_sprite.setTextureRect(this->_rect);
}

void SpriteSheet::setSpriteType(int line) {
	if (line >= this->_nbrLines)
		throw std::runtime_error("Erreur : Impossible de changer le sprite");
	this->_rect.top = line * this->_sizeElements.y;
	this->_rect.left = 0;
	this->_sprite.setTextureRect(this->_rect);
}

void SpriteSheet::nextSprite() {
	if (this->_nbrColumns == (this->_rect.left + this->_sizeElements.x) / this->_sizeElements.x) {
		this->_rect.left = 0;
	} else {
		this->_rect.left += this->_sizeElements.x;
	}
	this->_sprite.setTextureRect(this->_rect);
}

void SpriteSheet::setPos(sf::Vector2f pos) {
	this->_sprite.setPosition(pos);
}

void SpriteSheet::setTransparency() {
	sf::Color color = this->_sprite.getColor();
	color.a = 135;
	this->_sprite.setColor(color);
}

std::pair<int, int> SpriteSheet::getRectPosition() const {
	return std::make_pair<int, int>(
		this->_rect.left / this->_sizeElements.x + 1,
		this->_rect.top / this->_sizeElements.y + 1
	);
}

sf::Vector2f SpriteSheet::getSpritePosition() const {
	return this->_sprite.getPosition();
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
