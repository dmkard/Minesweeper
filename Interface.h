#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"
class Interface: public sf::Drawable
{
public:
	Interface();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


};

