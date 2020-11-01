#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Interface.h"
#include <vector>
#include <random>

class Game
{
public:
	enum class TileType {
						empty_tile = 0,
						tile_1 = 1,
						tile_2 = 2,
						tile_3 = 3,
						tile_4 = 4,
						tile_5 = 5,
						tile_6 = 6,
						tile_7 = 7, 
						tile_8 = 8,
						pressed_tile = 9,
						flagged_tile = 10,
						primary_tile = 11,
						has_mine_tile = 12,
						wrong_mine_tile = 13,
						mine_exploded_tile =14
						};
	Game();
	void Run();
	void HandleInput();
	void Update();
	void Render();

private:
	void loadTileTextures();
	void createGameField();
	void resetGameField();
	void initializeMine(const sf::Vector2i& startGridCoord);
	void revealTile(const sf::Vector2i& tileGridCoord);
	bool isValidGridCoord(const sf::Vector2i& tileGridCoord);
	bool isResetButton(const sf::Vector2i& eventCoord);
	sf::Vector2i coordToGridCoord(const sf::Vector2i& coord);
	void rightMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonPressed(const sf::Vector2i& eventCoord);
	void leftMouseButtonReleased(const sf::Vector2i& eventCoord);
	void countAmountMineNear();
	void revealTilesNear(const sf::Vector2i& eventGridCoord);
	Tile& tileAt(sf::Vector2i& gridCoord);
	Tile& tileAt(const sf::Vector2i& gridCoord);
	sf::RenderWindow window_;
	
	std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
	Interface interface_;

	int gameStopwatch_;
	short mine_amount_;
	short tile_revealed_amount_;
	bool running_;
	bool game_started_;
	bool game_over_;
	short board_rows_{ 16 };
	short board_columns_{ 30 };
	
};

