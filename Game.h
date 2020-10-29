#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <vector>
#include <random>

class Game
{
public:
	enum class TileType {
						tile = 0,
						tile_1 = 1,
						tile_2 = 2,
						tile_3 = 3,
						tile_4 = 4,
						tile_5 = 5,
						tile_6 = 6,
						tile_7 = 7, 
						tile_8 = 8,
						pressed_tile = 9,
						flagged_tile = 10
						};
	Game();
	void Run();
	void HandleInput();
	void Update();
	void Render();

private:
	void loadTileTextures();
	void createGameField();
	void initializeMine(const int& index);
	void revealTile(const int& index);
	void rightMouseButtonPressed(const int & x_coord, const int& y_coord);
	void leftMouseButtonPressed(const int& x_coord, const int& y_coord);
	void leftMouseButtonReleased(const int& x_coord, const int& y_coord);
	void countAmountMineNear();
	bool isValidTile(const int& x, const int& y) const;
	sf::RenderWindow window_;
	std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
	short mine_amount_;
	bool running_;
	bool game_started_;
	short board_rows_{ 16 };
	short board_columns_{ 30 };
	short margin_{ (W_WIDTH - 30 * TILE_SIDE_SIZE) / 2 };
};

