#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"
class Interface: public sf::Drawable
{
public:
	Interface();
	void setStopwatch(const int& gameTime);
	void setMineAmount(const int& mineAmount);
	sf::Color baseColor();
	void showCongatulations();
	void showRegrets();
	void resetTitle();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setupTextLabels();
	void setupBackground();
	void setupResetButton();
	void setupTitleText();

	const std::vector<std::string> titles_ = { "Good job", "Nice try" };
	sf::Texture button_texture_;

	sf::Font font_;

	sf::Text text_box_;
	sf::Text stopwatch_;
	sf::Text mine_left_;
	sf::RectangleShape background_;
	sf::RectangleShape reset_button_;
	sf::Color base_color_;
};

