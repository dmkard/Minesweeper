#include "Tile.h"

Tile::Tile(int row, int column, int side_length): tile_(sf::Vector2f(static_cast<float>(side_length), static_cast<float>(side_length)))
{

	tile_.setPosition(column * side_length + 50, row * side_length + 50);
}


void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(tile_);
}

void Tile::setTexture(sf::Texture& texture)
{
	tile_.setTexture(&texture);
}
