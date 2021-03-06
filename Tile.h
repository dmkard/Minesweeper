#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include "Const.h"

class Tile : public sf::Drawable
{
public:
	enum class State{primary = 1, flagged, revealed, pressed};
	Tile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void setTexture(const std::shared_ptr<sf::Texture>& texture);
	virtual void setPosition(const sf::Vector2f& position);

	bool hasMine();
	void changeTileState(State state);
	void setMine();
	void resetTile();
	State state();
	void incrementMineNearCounter();
	int8_t amountBombNear();
	Tile& at(sf::Vector2i gridCoord);
private:
	
	sf::RectangleShape tile_;
	std::shared_ptr<sf::Texture> texture_;
	int8_t amount_mine_near_;
	bool hasMine_;
	State state_;
};

