#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"

/**
 * Represent graphical appearance of the game
 *
 * It derives after sf::Drawable class and have to be drawed with a draw() function of sf::RenderWindow class
 * 
 */

class Interface: public sf::Drawable
{
public:
	//Default constructor
	Interface();
	//Formats and sets a time of a game 
	void setStopwatch(const int& gameTime);
	//Formats and sets a mine amount 
	void setMinesToFindAmount(const int& minesToFindAmount);
	//Checks if a point with a eventCoord coordinates is on a reset buttor
	bool isResetButton(const sf::Vector2i& eventCoord);
	//Resets a stopwatch and mine counter
	void resetCounters();
	//Returns a background of a window
	sf::Color backgroundColor();

	// Not copyable and not moveable.
	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;
	Interface(Interface&&) = delete;
	Interface& operator=(Interface&&) = delete;

private:
	//Draws all of the elements of interface
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	//Setups default colors
	void setupColorPalette();
	//Setups stopwatch's and mine counter's labels
	void setupTextLabels();
	//Setups background of a board
	void setupBoardBackground();
	//Setups reset button
	void setupResetButton();

	sf::Font text_font_;
	sf::Text stopwatch_;
	sf::Text mines_to_find_left_;
	sf::RectangleShape board_background_;
	sf::RectangleShape reset_button_;

	sf::Color background_color_;
	sf::Color interface_color_;
	sf::Color board_background_board_color_;

	const unsigned short text_font_size_ = B_MARGIN * 3; // in pixels
	const unsigned short button_size_ = B_MARGIN * 4; // in pixels
};

