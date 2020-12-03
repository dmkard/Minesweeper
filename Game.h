#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Interface.h"
#include "ResourceManager.h"
#include <vector>
#include <random>

/*
* This class use two notation to represent coordinates:
* eventCoord and eventGridCoord.
* eventCoor represents common coordinates system with a (0,0) point in the top left corner
* eventGridCoord represents matrix coordinates for a two-dimensional tile array
*/

class Game
{
public:
	Game();
	void Run();
	void HandleInput();
	void Render();

private:
	//Creates a set of tiles
	void createGameField();
	//Resets the state of all components of game
	void resetGameField();
	//Generates mines in tiles
	void initializeMine(const sf::Vector2i& startGridCoord);
	//Reveals internal the state of a tile
	void revealTile(const sf::Vector2i& tileGridCoord);
	//Reveals internal the state of tiles near tile with eventGridCoord coordinates
	void revealTilesNear(const sf::Vector2i& eventGridCoord);
	//Stops the game and changes textures on all unflagged tiles
	void mineExploded(const sf::Vector2i& tileGridCoord);
	//Counts amount of mines around every tile on field
	void countAmountMineNear();

	//Handle mouse events
	void rightMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonReleased(const sf::Vector2i& eventCoord);
	void bothMouseButoonPressed(const sf::Vector2i& eventCoord);
	
	//Checks for tileGridCoord for being inside of field
	bool isValidGridCoord(const sf::Vector2i& tileGridCoord);
	//Returns reference of a tile with a gridCoord
	Tile& tileAt(sf::Vector2i& gridCoord);
	Tile& tileAt(const sf::Vector2i& gridCoord);
	//Converts coord to gridCoord
	sf::Vector2i coordToGridCoord(const sf::Vector2i& coord);
	
	sf::RenderWindow window_;
	Interface interface_;
	
	std::vector<Tile> tiles_;

	//Holds gridCoord of last pressed tile under left mouse button
	//to change state under constatnly pressed left mouse button 
	sf::Vector2i previousPressedTileGridCoord_;
	
	int gameStopwatch_;
	//Holds planned to find amount of mines
	//If some of the mines were flagged wrogly, this value won't equal actual amount of mines to find
	short mines_to_find_amount_;
	//Holds the amount of revealed tiles
	short tile_revealed_amount_;

	
	bool window_is_running_;
	//Used to check if game have begun
	bool game_started_;
	//Used to prevent from clicking on field if game overed
	bool game_over_;

	//Change its state if there are any change on game field
	//which have to renred
	bool field_was_changed_;

};

