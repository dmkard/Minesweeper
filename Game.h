#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Interface.h"
#include "ResourceManager.h"
#include <vector>
#include <random>

class Game
{
public:
	Game();
	void Run();
	void HandleInput();
	void Render();

private:
	/*void loadTileTextures();*/
	void createGameField();
	void resetGameField();
	void initializeMine(const sf::Vector2i& startGridCoord);
	void revealTile(const sf::Vector2i& tileGridCoord);
	bool isValidGridCoord(const sf::Vector2i& tileGridCoord);
	bool isResetButton(const sf::Vector2i& eventCoord);
	sf::Vector2i coordToGridCoord(const sf::Vector2i& coord);

	void mineExploded(const sf::Vector2i& tileGridCoord);
	void rightMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonReleased(const sf::Vector2i& eventCoord);
	void bothMouseButoonPressed(const sf::Vector2i& eventCoord);
	void revealTilesNear(const sf::Vector2i& eventGridCoord);
	void countAmountMineNear();

	Tile& tileAt(sf::Vector2i& gridCoord);
	Tile& tileAt(const sf::Vector2i& gridCoord);

	sf::RenderWindow window_;
	Interface interface_;
	sf::Image icon_;

	//std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
	

	sf::Vector2i previousPressedTile_{ -1,-1 };
	
	int gameStopwatch_;
	short mine_amount_;
	short tile_revealed_amount_;
	bool running_;
	bool game_started_;
	bool game_over_;
	bool field_was_changed_;

};

