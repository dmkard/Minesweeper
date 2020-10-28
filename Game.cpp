#include "Game.h"

Game::Game() : running_{ false },
                window_(sf::VideoMode(1300, 800), "Minesweeper"), 
                bomb_amount_{ 0 }, 
                board_rows_{ 16 },
                board_columns_{ 30 }
{
    LoadTileTextures();
}

void Game::Run()
{
    running_ = true;
    CreateGameField(40);
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
    for (int i = 1; i < 9; ++i)
    {
        texture.loadFromFile("resources/pictures/tile" + std::to_string(i) + ".png");
        tile_textures_.emplace_back(texture);
    }
    texture.loadFromFile("resources/pictures/pressed_tile.png");
    tile_textures_.emplace_back(texture);

    texture.loadFromFile("resources/pictures/bomb_tile.png");
    tile_textures_.emplace_back(texture);
}

void Game::CreateGameField(int side_length)
{
    //it creates tiles and sets default texture to each of them
    for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j <board_columns_; ++j)
        {
            Tile tile = Tile(i, j, side_length);
            tile.setTexture(tile_textures_[static_cast<int>(TileType::tile)]);
            tiles_.emplace_back(tile);
        }
    }

    InitializeBombs();

    //loops for testing bomb initializetion
    /*for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j < board_columns_; ++j)
        {
            if(tiles_[i * board_columns_ + j].isBomb())
                tiles_[i*board_columns_+ j].setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
        }
    }*/
}

void Game::InitializeBombs()
{
    std::default_random_engine engine{};
    std::uniform_int_distribution<int> r_row_generator(0,board_rows_-1);
    std::uniform_int_distribution<int> r_columns_generator(0, board_columns_-1);

    while (bomb_amount_ < 99)
    {
        int r_row = r_row_generator(engine);
        int r_column = r_columns_generator(engine);
        int index = r_row * board_columns_ + r_column; //calculate index in order to access to correct tile in vector of tiles
        if (!tiles_[index].isBomb())
        {
            tiles_[index].setBomb();
            ++bomb_amount_;
        }  
    }
}
