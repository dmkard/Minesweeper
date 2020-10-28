#include "Game.h"
#include "Const.h"
Game::Game() :  running_{ false },
                game_started_{false},
                window_(sf::VideoMode(W_WIDTH, W_HEIGHT), "Minesweeper"), 
                bomb_amount_{ 0 }
{
    LoadTileTextures();
}

void Game::Run()
{
    running_ = true;
    CreateGameField();
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

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "the right button was pressed" << std::endl;
                RightMouseButtonPressed(event.mouseButton.x, event.mouseButton.y);
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "the left button was pressed" << std::endl;
                LeftMouseButtonPressed(event.mouseButton.x, event.mouseButton.y);
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "the left button was released" << std::endl;
                LeftMouseButtonReleased(event.mouseButton.x, event.mouseButton.y);
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
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

void Game::CreateGameField()
{
    //it creates tiles and sets default texture to each of them
    for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j <board_columns_; ++j)
        {
            Tile tile{};

            tile.setTexture(tile_textures_[static_cast<int>(TileType::tile)]);

            sf::Vector2f position({ static_cast<float>( j * TILE_SIDE_SIZE + margin_) },
                                    { static_cast<float>(i * TILE_SIDE_SIZE + margin_) });
            tile.setPosition(position);

            tiles_.emplace_back(tile);
        }
    }
}

void Game::InitializeBombs(const int& initial_index)
{
    bomb_amount_ = 0;
    std::default_random_engine engine{};
    std::uniform_int_distribution<int> r_row_generator(0,board_rows_-1);
    std::uniform_int_distribution<int> r_columns_generator(0, board_columns_-1);

    while (bomb_amount_ < 99)
    {
        int r_row = r_row_generator(engine);
        int r_column = r_columns_generator(engine);
        int index = r_row * board_columns_ + r_column; //calculate index in order to access to correct tile in vector of tiles

        //this bool variable holds information if the index is in a tile radius around a pressed tile
        //if so, this index have to be skipped, in order to initialize basic condition of the game properly
        bool isAroundFirstTile = (  (index == initial_index + 1) ||
                                    (index == initial_index - 1) ||
                                    (index == initial_index + board_columns_) ||
                                    (index == initial_index - board_columns_) ||
                                    (index == initial_index + 1 + board_columns_) ||
                                    (index == initial_index - 1 + board_columns_) ||
                                    (index == initial_index + 1 - board_columns_) ||
                                    (index == initial_index - 1 - board_columns_));

        if (!isAroundFirstTile && !tiles_[index].isBomb())
        {
            tiles_[index].setBomb();
            ++bomb_amount_;
        }  
    }
    //loops for testing bomb initializetion
    /*
    for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j < board_columns_; ++j)
        {
            if (tiles_[i * board_columns_ + j].isBomb())
                tiles_[i * board_columns_ + j].setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
        }
    }*/
}

void Game::RevealTile(const int& index)
{
    //opening of the first tile on board start the game
    if (!game_started_)
    {
        game_started_ = true;
        InitializeBombs(index);
    }
    tiles_[index].setTexture(tile_textures_[static_cast<int>(TileType::tile_3)]);
}

void Game::RightMouseButtonPressed(const int& x_coord, const int& y_coord)
{
    int column = (x_coord - margin_) / TILE_SIDE_SIZE; 
    int row = (y_coord - margin_) / TILE_SIDE_SIZE;

    if ((column >= 0 && column < board_columns_ )
            && (row >= 0 && row < board_rows_))
    {
        int index = row * board_columns_ + column;
        if (tiles_[index].state() == Tile::State::primary)
        {
            tiles_[index].setTexture(tile_textures_[static_cast<int>(TileType::bomb_tile)]);
            tiles_[index].changeTileState(Tile::State::flagged);
        }
        else if (tiles_[index].state() == Tile::State::flagged)
        {
            tiles_[index].setTexture(tile_textures_[static_cast<int>(TileType::tile)]);
            tiles_[index].changeTileState(Tile::State::primary);
        }
    }
}

void Game::LeftMouseButtonPressed(const int& x_coord, const int& y_coord)
{
    int column = (x_coord - margin_) / TILE_SIDE_SIZE;
    int row = (y_coord - margin_) / TILE_SIDE_SIZE;

    if ((column >= 0 && column < board_columns_)
        && (row >= 0 && row < board_rows_))
    {
        int index = row * board_columns_ + column;
        if (tiles_[index].state() == Tile::State::primary)
        {
            tiles_[index].setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
            tiles_[index].changeTileState(Tile::State::pressed);
        }     
    }
}

void Game::LeftMouseButtonReleased(const int& x_coord, const int& y_coord)
{
    int column = (x_coord - margin_) / TILE_SIDE_SIZE;
    int row = (y_coord - margin_) / TILE_SIDE_SIZE;
    int index = row * board_columns_ + column;

    //checking for out of board and if a tile is pressed, in order to finish sequence: pressed->released
    if ((column >= 0 && column < board_columns_)
        && (row >= 0 && row < board_rows_)
        && (tiles_[index].state() == Tile::State::pressed))
    {
        RevealTile(index);
    }
    else//one tile was pressed, so program have to find it in order to finish sequence: pressed->released
    {
        for (unsigned  i = 0; i < tiles_.size(); ++i)
        {
            if (tiles_[i].state() == Tile::State::pressed)
                RevealTile(i);
        }
    }
}
