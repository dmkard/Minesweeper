#include "Game.h"

Game::Game(): running_{false}, window_(sf::VideoMode(1300, 800), "Minesweeper")
{
    LoadTileTextures();
}

void Game::Run()
{
    running_ = true;
    CreateTiles(16, 30, 40);
    while (running_)
    {
        HandleInput();
        Update();
        Render();
    }
}

void Game::HandleInput()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window_.close();
            running_ = false;
        }    
    }
}

void Game::Update()
{
}

void Game::Render()
{
    window_.clear(sf::Color(37, 52, 35));
    for (Tile tile : tiles_)
        window_.draw(tile);
    window_.display();
}

void Game::LoadTileTextures()
{
    sf::Texture texture;
    texture.loadFromFile("resources/pictures/tile.png");
    tile_textures_.emplace_back(texture);
    for (int i = 1; i <= 9; ++i)
    {
        texture.loadFromFile("resources/pictures/tile" + std::to_string(i) + ".png");
        tile_textures_.emplace_back(texture);
    }
    texture.loadFromFile("resources/pictures/pressed_tile.png");
    tile_textures_.emplace_back(texture);

    texture.loadFromFile("resources/pictures/bomb_tile.png");
    tile_textures_.emplace_back(texture);
}

void Game::CreateTiles(int  rows, int columns, int side_length)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j <columns; ++j)
        {
            Tile tile = Tile(i, j, side_length);
            tile.setTexture(tile_textures_[10]);
            tiles_.emplace_back(tile);
        }
    }
}
