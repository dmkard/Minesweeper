#include "Tile.h"


Tile::Tile(): tile_(sf::Vector2f(static_cast<float>(TILE_SIDE_SIZE), static_cast<float>(TILE_SIDE_SIZE))),
													hasMine_{false},
													state_{State::primary},
													amount_mine_near_{0},
													texture_{nullptr}
{}


void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(tile_);
}

void Tile::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
	texture_.reset();
	texture_ = texture;
	tile_.setTexture(texture_.get());
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

void Tile::resetTile()
{
	hasMine_ = false;
	state_ = State::primary;
	amount_mine_near_ = 0;
}

Tile::State Tile::state()
{
	 return state_;
}

void Tile::incrementMineNearCounter()
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
