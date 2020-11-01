#include "Interface.h"

Interface::Interface()
{
}

void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(tile_);
}
