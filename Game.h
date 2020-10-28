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
						bomb_tile = 10
						};
	Game();
	void Run();
	void HandleInput();
	void Update();
	void Render();

private:
	void LoadTileTextures();
	void CreateGameField();
	void InitializeBombs(const int& index);
	void RevealTile(const int& index);
	void RightMouseButtonPressed(const int & x_coord, const int& y_coord);
	void LeftMouseButtonPressed(const int& x_coord, const int& y_coord);
	void LeftMouseButtonReleased(const int& x_coord, const int& y_coord);
	sf::RenderWindow window_;
	std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
	short bomb_amount_;
	bool running_;
	bool game_started_;
	short board_rows_{ 16 };
	short board_columns_{ 30 };
	short margin_{ (W_WIDTH - 30 * TILE_SIDE_SIZE) / 2 };
};

