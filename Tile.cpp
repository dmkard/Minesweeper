#include "Tile.h"

Tile::Tile(): tile_(sf::Vector2f(static_cast<float>(TILE_SIDE_SIZE), static_cast<float>(TILE_SIDE_SIZE))),
													isBomb_{false},
													state_{State::primary}
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
