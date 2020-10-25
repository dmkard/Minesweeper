#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Tile : public sf::Drawable
{
public:
	Tile(int row, int column, int side_length);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setTexture(sf::Texture& texture);
private:
	sf::RectangleShape tile_;
	sf::Texture texture_;
};

