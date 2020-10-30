#include "Game.h"
#include "Const.h"
#include <cassert>
#include <algorithm>
#include <chrono>
Game::Game() :  running_{ false },
                game_started_{false},
                window_(sf::VideoMode(W_WIDTH, W_HEIGHT), "Minesweeper"), 
                mine_amount_{ 0 }
{
    loadTileTextures();
}
//A funtion with game loop
void Game::Run()
{
    running_ = true;
    createGameField();
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
                rightMouseButtonPressed({ event.mouseButton.x, event.mouseButton.y });
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "the left button was pressed" << std::endl;
                leftMouseButtonPressed({ event.mouseButton.x, event.mouseButton.y });
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "the left button was released" << std::endl;
                leftMouseButtonReleased({ event.mouseButton.x, event.mouseButton.y });
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

//A function loads all tile textures to vector with textures
void Game::loadTileTextures()
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

    texture.loadFromFile("resources/pictures/flagged_tile.png");
    tile_textures_.emplace_back(texture);
}

//A function create board_rows_*board_columns_ tiles
void Game::createGameField()
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

//A function randomly sets 99 Mine on early created tites
void Game::initializeMine(const sf::Vector2i& startGridCoord)
{
    mine_amount_ = 0;
    std::default_random_engine engine{};
    std::uniform_int_distribution<int> r_row_generator(0,board_rows_-1);
    std::uniform_int_distribution<int> r_columns_generator(0, board_columns_-1);

    while (mine_amount_ < 99)
    {
        sf::Vector2i randomMinePos{ r_columns_generator(engine), r_row_generator(engine) };
        /*
        int r_row = r_row_generator(engine);
        int r_column = r_columns_generator(engine);*/
        int index = randomMinePos.y * board_columns_ + randomMinePos.x; //calculate index in order to access to correct tile in vector of tiles
        int initial_index = startGridCoord.y * board_columns_ + startGridCoord.x;
        //this bool variable holds information if the index is in a tile radius around a pressed tile
        //if so, this index have to be skipped, in order to initialize basic condition of the game properly
        bool isAroundFirstTile = (  (index == initial_index + 1) ||
                                    (index == initial_index - 1) ||
                                    (index == initial_index + board_columns_) ||
                                    (index == initial_index - board_columns_) ||
                                    (index == initial_index + 1 + board_columns_) ||
                                    (index == initial_index + 1 - board_columns_) ||
                                    (index == initial_index - 1 + board_columns_) ||
                                    (index == initial_index - 1 - board_columns_));
                                    
        /*bool isAroundFirstTile = (  (randomMinePos == initial_index + 1) ||
                                    (index == initial_index - 1) ||
                                    (index == initial_index + board_columns_) ||
                                    (index == initial_index - board_columns_) ||
                                    (index == initial_index + 1 + board_columns_) ||
                                    (index == initial_index + 1 - board_columns_) ||
                                    (index == initial_index - 1 + board_columns_) ||
                                    (index == initial_index - 1 - board_columns_));*/

        if (!isAroundFirstTile && !tileAt(randomMinePos).hasMine())
        {
            tileAt(randomMinePos).setMine();
            ++mine_amount_;
        }  
    }
}

//A function reveal tile after a left mouse button click
void Game::revealTile(const sf::Vector2i& tileGridCoord)
{
    //opening of the first tile on board start the game
    if (!game_started_)
    {
        game_started_ = true;
        auto start = std::chrono::high_resolution_clock::now();
        initializeMine(tileGridCoord);
        auto end = std::chrono::high_resolution_clock::now();
        countAmountMineNear();
        
        std::cout << "Tile spent to a bombs initialization: " <<
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "mcs\n";
        
        //testing loop
        
        for (unsigned i = 0; i < tiles_.size(); ++i)
        {
            if (tiles_[i].hasMine())
                tiles_[i].setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
            else
                tiles_[i].setTexture(tile_textures_[tiles_[i].amountBombNear()]);
        }
    }
    //tileAt(tileGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::tile_3)]);
}

bool Game::isValidGridCoord(const sf::Vector2i& tileGridCoord)
{
    return tileGridCoord.x >=0 && tileGridCoord.x < board_columns_
            && tileGridCoord.y >=0 && tileGridCoord.y < board_rows_;
}

 sf::Vector2i Game::coordToGridCoord(const sf::Vector2i& coord)
{
    return { (coord.x - margin_) / TILE_SIDE_SIZE, (coord.y - margin_) / TILE_SIDE_SIZE };
}


//A function changes state and texture after right mousebuttonpressed event
void Game::rightMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::flagged_tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::flagged);
        }
        else if (tileAt(eventGridCoord).state() == Tile::State::flagged)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::primary);
        }
    }
}

//A function changes state and texture on tile after left mousebutttonpressed event
void Game::leftMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::pressed);
        }     
    }
}

//A function changes state of a tile and reveal it after left mousebutttonreleasef event
//but if user will move mouse to another tile after pressing on some tile, pressed one will be revealed 
void Game::leftMouseButtonReleased(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (isValidGridCoord(eventGridCoord) && tileAt(eventGridCoord).state() == Tile::State::pressed)
    {
        revealTile(eventGridCoord);
    }
    else//one tile was pressed, so program have to find it in order to finish sequence: pressed->released
    {
        for (unsigned  i = 0; i < tiles_.size(); ++i)
        {
            if (tiles_[i].state() == Tile::State::pressed)
                revealTile(eventGridCoord);
        }
    }
}

void Game::countAmountMineNear()
{
    int xStart{}, yStart{};
    int xEnd{}, yEnd{};

    for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j < board_columns_; ++j)
        {
            if (tileAt({ j, i }).hasMine()) 
            {
                //finding a range of cheching to prevent out of range access
                xStart = std::max(0, j - 1);
                yStart = std::max(0, i - 1);
                xEnd = std::min(j + 1, board_columns_ - 1);
                yEnd = std::min(i + 1, board_rows_ - 1);
                for (int yPos = yStart; yPos <= yEnd; ++yPos)
                {
                    for (int xPos = xStart; xPos <= xEnd; ++xPos)
                    {
                        if (!tileAt({ xPos, yPos }).hasMine())
                            tileAt({ xPos, yPos }).increaseBombNear();
                    }
                }
            }
        }
    }
}

//A function returns a reference to a tile with (x,y) coordinates
Tile& Game::tileAt(sf::Vector2i& gridCoord)
{
    return tiles_[gridCoord.y * board_columns_ + gridCoord.x];
}


Tile& Game::tileAt(const sf::Vector2i& gridCoord) 
{
    return tiles_[gridCoord.y * board_columns_ + gridCoord.x];
}
