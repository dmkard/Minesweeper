#include "Tile.h"

Tile::Tile(): tile_(sf::Vector2f(static_cast<float>(TILE_SIDE_SIZE), static_cast<float>(TILE_SIDE_SIZE))),
													hasMine_{false},
													state_{State::primary},
													amount_mine_near_{0}
{}


void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(tile_);
}

void Tile::setTexture(sf::Texture& texture)
{
	tile_.setTexture(&texture);
}

void Tile::setPosition(const sf::Vector2f& position)
{
	tile_.setPosition(position);
}

bool Tile::hasMine()
{
	return hasMine_;
}

void Tile::changeTileState(State state)
{
	state_ = state;
}

void Tile::setMine()
{
	hasMine_ = true;
}

Tile::State Tile::state()
{
	 return state_;
}

void Tile::increaseBombNear()
{
	++amount_mine_near_;
}

int8_t Tile::amountBombNear()
{
	return amount_mine_near_;
}

Tile& Tile::at(sf::Vector2i gridCoord)
{
	return *this;
}
