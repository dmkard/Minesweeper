#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Const.h"

class Tile : public sf::Drawable
{
public:
	enum class State{primary = 1, flagged, opened, pressed};
	Tile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void setTexture(sf::Texture& texture);
	virtual void setPosition(const sf::Vector2f& position);
	bool isBomb() { return isBomb_; }
	void changeTileState(State state) { state_ = state; }
	void setBomb() { isBomb_ = true; }
	State state() { return state_; }
private:
	
	sf::RectangleShape tile_;
	sf::Texture texture_;
	bool isBomb_;
	State state_;
};

