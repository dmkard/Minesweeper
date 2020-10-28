#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Tile : public sf::Drawable
{
public:
	enum class State{main = 1, flagged, opened};
	Tile(int row, int column, int side_length);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setTexture(sf::Texture& texture);
	bool isBomb() { return isBomb_; }
	void setBomb() { isBomb_ = true; }
private:
	
	sf::RectangleShape tile_;
	sf::Texture texture_;
	bool isBomb_;
	State state_;
};

