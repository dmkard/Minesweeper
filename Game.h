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
						flagged_tile = 9,
						primary_tile = 10,
						has_mine_tile = 11,
						wrong_mine_tile = 12,
						mine_exploded_tile =13
						};
	Game();
	void Run();
	void HandleInput();
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
	void bothMouseButoonPressed(const sf::Vector2i& eventCoord);
	void doActionOnArea(void (Game::*f)(const sf::Vector2i& eventGridCoord), const sf::Vector2i& eventGridCoord);
	void countAmountMineNear();

	Tile& tileAt(sf::Vector2i& gridCoord);

	Tile& tileAt(const sf::Vector2i& gridCoord);
	sf::RenderWindow window_;
	sf::Image icon_;

	std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
	Interface interface_;

	sf::Vector2f previousPressed_{ 0,0 };
	
	int gameStopwatch_;
	short mine_amount_;
	short tile_revealed_amount_;
	bool running_;
	bool game_started_;
	bool game_over_;
	bool field_was_changed_;

};

