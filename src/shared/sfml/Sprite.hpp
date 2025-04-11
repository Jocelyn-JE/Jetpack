//
// Created by roussierenoa on 4/11/25.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SFML/Graphics.hpp>

class Sprite {
private:
	sf::Texture _texture;
	sf::Sprite _sprite;
public:
   	void display(sf::RenderWindow &window);

	void changePos(sf::Vector2f pos);

	sf::Vector2f getPos();

	Sprite(const std::string &path);

	Sprite(const std::string &path, sf::Vector2f pos, sf::Vector2f scale);

    ~Sprite() = default;
};



#endif //IMAGE_HPP
