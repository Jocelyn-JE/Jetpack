//
// Created by roussierenoa on 4/11/25.
//

#ifndef SRC_SHARED_SFML_SPRITESHEET_HPP_
#define SRC_SHARED_SFML_SPRITESHEET_HPP_

#include <string>
#include <utility>

#include <SFML/Graphics.hpp>

class SpriteSheet {
 private:
    sf::Vector2i _sizeElements;
    int _nbrLines;
    int _nbrColumns;

    sf::IntRect _rect;
    sf::Texture _texture;
    sf::Sprite _sprite;

 public:
    void display(sf::RenderWindow &window);

    void setSprite(int col, int line);

    void setSpriteType(int col);

    std::pair<int, int> getRectPosition() const;

    sf::Vector2f getSpritePosition() const;

    void nextSprite();

    void setPos(sf::Vector2f pos);

    void setTransparency();

    SpriteSheet(const std::string &path, sf::Vector2i sizeElements,
                int nbrLines, int nbrColumns, sf::Vector2f scale = {1, 1});
    ~SpriteSheet() = default;
};

#endif  // SRC_SHARED_SFML_SPRITESHEET_HPP_
