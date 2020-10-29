#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>
#include "Const.h"

class Tile : public sf::Drawable
{
public:
	enum class State{primary = 1, flagged, revealed, pressed};
	Tile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void setTexture(sf::Texture& texture);
	virtual void setPosition(const sf::Vector2f& position);
	bool hasMine() { return hasMine_; }
	void changeTileState(State state) { state_ = state; }
	void setMine() { hasMine_ = true; }
	State state() { return state_; }
	void increaseBombNear() { ++amount_mine_near_; }
	int8_t amountBombNear() { return amount_mine_near_; }
private:
	
	sf::RectangleShape tile_;
	sf::Texture texture_;
	int8_t amount_mine_near_;
	bool hasMine_;
	State state_;
};

