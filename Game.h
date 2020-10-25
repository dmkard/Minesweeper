#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <vector>

class Game
{
public:
	enum class TileType {
						regular_tile = 0,
						tile_1 = 1,
						tile_2 = 2,
						tile_3 = 3,
						tile_4 = 4,
						tile_5 = 5,
						tile_6 = 6,
						tile_7 = 7, 
						tile_8 = 8,
						pressed_tile = 9
						};
	Game();
	void Run();
	void HandleInput();
	void Update();
	void Render();

private:
	void LoadTileTextures();
	void CreateTiles(int  rows, int columns, int side_length);

	sf::RenderWindow window_;
	bool running_;
	std::vector<sf::Texture> tile_textures_;
	std::vector<Tile> tiles_;
};

