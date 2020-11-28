#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"
class Interface: public sf::Drawable
{
public:
	Interface();
	void setStopwatch(const int& gameTime);
	void setMineAmount(const int& mineAmount);
	void showCongatulations();
	void showRegrets();
	void resetTitle();
	sf::Color baseColor();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setupColorPalette();
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
	sf::Color interface_color_;
	sf::Color background_board_color_;
	const unsigned short font_size_ = B_MARGIN * 3;
};

