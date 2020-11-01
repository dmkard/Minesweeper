#include "Game.h"
#include "Const.h"
#include <cassert>
#include <algorithm>
#include <chrono>
Game::Game() :  running_{ false },
                game_started_{false},
                game_over_{false},
                window_(sf::VideoMode(W_WIDTH, W_HEIGHT), "Minesweeper", sf::Style::Titlebar | sf::Style::Close),
                mine_amount_{ MINE_AMOUNT },
                gameStopwatch_{0},
                interface_{}
                
{
    loadTileTextures();
}
//A funtion with game loop
void Game::Run()
{
    
    sf::Clock clock;
    sf::Time endFrameTime;
    int frameTime = sf::seconds(1).asMilliseconds() / FRAMERATE;
    int frameCounter{};
    running_ = true;
    createGameField();
    std::cout << sizeof(tiles_[1]) << std::endl;
    std::cout << sizeof(sf::RectangleShape) << std::endl;
    std::cout << sizeof(sf::Texture) << std::endl;
    std::cout << sizeof(int8_t) << std::endl;
    std::cout << sizeof(bool) << std::endl;
    std::cout << sizeof(Tile::State) << std::endl;
    std::cout << sizeof(tile_textures_[1]) << std::endl;



    while (running_)
    {
        clock.restart();

        if (game_started_)
        {
            ++frameCounter;
            if (frameCounter % FRAMERATE == 0)
            {
                ++gameStopwatch_;
                interface_.setStopwatch(gameStopwatch_);
            }
        }
        interface_.setMineAmount(mine_amount_);
        HandleInput();
        Update();
        Render();
        endFrameTime = clock.getElapsedTime(); //get elated time from beginning of frame
        if (endFrameTime.asMilliseconds() < frameTime)
            sf::sleep(sf::milliseconds(frameTime - endFrameTime.asMilliseconds()));
        
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
                rightMouseButtonPressed({ event.mouseButton.x, event.mouseButton.y });

            if (event.mouseButton.button == sf::Mouse::Left)
                leftMouseButtonPressed({ event.mouseButton.x, event.mouseButton.y });
        }
        if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
                leftMouseButtonReleased({ event.mouseButton.x, event.mouseButton.y });
        }
    }
}

void Game::Update()
{
}

void Game::Render()
{
    window_.clear(interface_.baseColor());
    window_.draw(interface_);
    for (Tile tile : tiles_)
        window_.draw(tile);
    window_.display();
}

//A function loads all tile textures to vector with textures
void Game::loadTileTextures()
{
    sf::Texture texture;
    texture.loadFromFile("resources/pictures/empty_tile.png");
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

    texture.loadFromFile("resources/pictures/primary_tile.png");
    tile_textures_.emplace_back(texture);

    texture.loadFromFile("resources/pictures/has_mine_tile.png");
    tile_textures_.emplace_back(texture);

    texture.loadFromFile("resources/pictures/wrong_mine_tile.png");
    tile_textures_.emplace_back(texture);

    texture.loadFromFile("resources/pictures/mine_exploded_tile.png");
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

            tile.setTexture(tile_textures_[static_cast<int>(TileType::primary_tile)]);

            sf::Vector2f position({ static_cast<float>( j * TILE_SIDE_SIZE + margin) },
                                    { static_cast<float>(i * TILE_SIDE_SIZE + margin) });
            tile.setPosition(position);

            tiles_.emplace_back(tile);
        }
    }
}

void Game::resetGameField()
{
    game_started_ = false;
    game_over_ = false;
    mine_amount_ = MINE_AMOUNT;
    gameStopwatch_ = 0;
    tile_revealed_amount_ = 0;
    interface_.setStopwatch(gameStopwatch_);
    interface_.resetTitle();
    for (int i = 0; i < board_rows_; ++i)
    {
        for (int j = 0; j < board_columns_; ++j)
        {
            Tile& tile = tileAt({ j, i });
            tile.resetTile();
            tile.setTexture(tile_textures_[static_cast<int>(TileType::primary_tile)]);
        }
    }
}

//A function randomly sets 99 Mine on early created tites
void Game::initializeMine(const sf::Vector2i& startGridCoord)
{
    int mineLeft = MINE_AMOUNT;
    std::random_device engine;
    std::uniform_int_distribution<int> r_row_generator(0,board_rows_-1);
    std::uniform_int_distribution<int> r_columns_generator(0, board_columns_-1);

    while (mineLeft > 0)
    {
        sf::Vector2i randomMinePos{ r_columns_generator(engine), r_row_generator(engine) };

        int index = randomMinePos.y * board_columns_ + randomMinePos.x; //calculate index in order to access to correct tile in vector of tiles
        int initial_index = startGridCoord.y * board_columns_ + startGridCoord.x;

        //this bool variable holds information if the index is in a tile radius around a pressed tile
        //if so, this index have to be skipped, in order to initialize basic condition of the game properly
        bool isAroundFirstTile = (  (index == initial_index) ||
                                    (index == initial_index + 1) ||
                                    (index == initial_index - 1) ||
                                    (index == initial_index + board_columns_) ||
                                    (index == initial_index - board_columns_) ||
                                    (index == initial_index + 1 + board_columns_) ||
                                    (index == initial_index + 1 - board_columns_) ||
                                    (index == initial_index - 1 + board_columns_) ||
                                    (index == initial_index - 1 - board_columns_));

        if (!isAroundFirstTile && !tileAt(randomMinePos).hasMine())
        {
            tileAt(randomMinePos).setMine();
            --mineLeft;
        }  
    }
}

//A function reveal tile after a left mouse button click
void Game::revealTile(const sf::Vector2i& tileGridCoord)
{
    if (tileAt(tileGridCoord).state() == Tile::State::primary)
    {
        //opening of the first tile on board start the game
        if (!game_started_)
        {
            game_started_ = true;
            initializeMine(tileGridCoord);
            countAmountMineNear();
        }
        
        tileAt(tileGridCoord).changeTileState(Tile::State::revealed);

        if (tileAt(tileGridCoord).hasMine())
        {
            game_started_ = false;
            game_over_ = true;
            interface_.showRegrets();
            int index_texture{};
            for (int i = 0; i < board_rows_; ++i)
            {
                for (int j = 0; j < board_columns_; ++j)
                {
                    if (tileAt({ j,i }).state() == Tile::State::flagged && !tileAt({ j,i }).hasMine())
                    {
                        index_texture = static_cast<int> (TileType::wrong_mine_tile);
                        tileAt({ j,i }).setTexture(tile_textures_[index_texture]);
                    }
                    else if(tileAt({ j,i }).hasMine() && tileAt({ j,i }).state() != Tile::State::flagged)
                    {
                        index_texture = static_cast<int> (TileType::has_mine_tile);
                        tileAt({ j,i }).setTexture(tile_textures_[index_texture]);
                    }
                }
            }

            index_texture = static_cast<int> (TileType::mine_exploded_tile);
            tileAt(tileGridCoord).setTexture(tile_textures_[index_texture]);
        }
        else
        {
            if (tileAt(tileGridCoord).amountBombNear() == 0)
            {
                ++tile_revealed_amount_;
                if (tile_revealed_amount_ == B_WIDTH * B_HEIGHT - MINE_AMOUNT)
                {
                    game_started_ = false;
                    game_over_ = true;
                    interface_.showCongatulations();
                }
                tileAt(tileGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::empty_tile)]);
                revealTilesNear(tileGridCoord);

            }
            else
            {
                ++tile_revealed_amount_;
                tileAt(tileGridCoord).setTexture(tile_textures_[tileAt(tileGridCoord).amountBombNear()]);
                if (tile_revealed_amount_ == B_WIDTH * B_HEIGHT - MINE_AMOUNT)
                {
                    game_started_ = false;
                    game_over_ = true;
                    interface_.showCongatulations();
                }
            }  
        }
    } 
}

void Game::revealTilesNear(const sf::Vector2i& eventGridCoord)
{
    int xStart = std::max(0, eventGridCoord.x - 1);
    int yStart = std::max(0, eventGridCoord.y - 1);
    int xEnd = std::min(eventGridCoord.x + 1, board_columns_ - 1);
    int yEnd = std::min(eventGridCoord.y + 1, board_rows_ - 1);

    for (int yPos = yStart; yPos <= yEnd; ++yPos)
    {
        for (int xPos = xStart; xPos <= xEnd; ++xPos)
        {
            revealTile({ xPos, yPos });
        }
    }
}

bool Game::isValidGridCoord(const sf::Vector2i& tileGridCoord)
{
    return tileGridCoord.x >=0 && tileGridCoord.x < board_columns_
            && tileGridCoord.y >=0 && tileGridCoord.y < board_rows_;
}

bool Game::isResetButton(const sf::Vector2i& eventCoord)
{
    return eventCoord.x > margin + TILE_SIDE_SIZE * (B_WIDTH - 1) &&
            eventCoord.x < margin + TILE_SIDE_SIZE * B_WIDTH &&
            eventCoord.y > 2.f * margin + TILE_SIDE_SIZE * B_HEIGHT &&
            eventCoord.y < 2.f * margin + TILE_SIDE_SIZE * (B_HEIGHT+1);
}

 sf::Vector2i Game::coordToGridCoord(const sf::Vector2i& coord)
{
    return { (coord.x - margin) / TILE_SIDE_SIZE, (coord.y - margin) / TILE_SIDE_SIZE };
}


//A function changes state and texture after right mousebuttonpressed event
void Game::rightMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (!game_over_ && isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::flagged_tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::flagged);
            --mine_amount_;
        }
        else if (tileAt(eventGridCoord).state() == Tile::State::flagged)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::primary_tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::primary);
            ++mine_amount_;
        }
    }
}

//A function changes state and texture on tile after left mousebutttonpressed event
void Game::leftMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (!game_over_ && isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(tile_textures_[static_cast<int>(TileType::pressed_tile)]);
            tileAt(eventGridCoord).changeTileState(Tile::State::pressed);
        }     
    }
    else if (isResetButton(eventCoord))
    {
        resetGameField();
    }

}

//A function changes state of a tile and reveal it after left mousebutttonreleasef event
//but if user will move mouse to another tile after pressing on some tile, pressed one will be revealed 
void Game::leftMouseButtonReleased(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);
    if (isValidGridCoord(eventGridCoord) &&
        tileAt(eventGridCoord).state() == Tile::State::pressed &&
        isValidGridCoord(eventGridCoord))
    {
        tileAt(eventGridCoord).changeTileState(Tile::State::primary);
        revealTile(eventGridCoord);
    }
    else//one tile was pressed, so program have to find it in order to finish sequence: pressed->released
    {
        for (int i = 0; i < board_rows_; ++i)
        {
            for (int j = 0; j < board_columns_; ++j)
            {
                if (tileAt({ j,i }).state() == Tile::State::pressed)
                {
                    tileAt({ j, i }).changeTileState(Tile::State::primary);
                    revealTile({ j, i });
                }    
            }
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
