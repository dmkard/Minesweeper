#include "Game.h"
#include "Const.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cmath>


Game::Game() :  window_is_running_{ false },
                game_started_{false},
                game_over_{false},
                field_was_changed_{true},
                window_(sf::VideoMode(W_WIDTH, W_HEIGHT), "Minesweeper", sf::Style::Titlebar | sf::Style::Close),
                mines_to_find_amount_{ MINE_AMOUNT },
                gameStopwatch_{0},
                interface_{},
                tile_revealed_amount_{0},
                previousPressedTileGridCoord_{ -1,-1 }
{ 
    sf::Image icon = *ResourceManager<sf::Image>::Instance().GetResource("resources/icon.png");
    window_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::Run()
{
    sf::Clock clock;
    sf::Time endFrameTime {};
    int frameTime = sf::seconds(1).asMilliseconds() / FRAMERATE;
    int frameCounter{};
    window_is_running_ = true;
    createGameField();
    //Game loop
    while (window_is_running_)
    {
        clock.restart();

        if (game_started_)
        {
            interface_.setMinesToFindAmount(mines_to_find_amount_);
            ++frameCounter;
            if (frameCounter % FRAMERATE == 0)
            {
                ++gameStopwatch_;
                interface_.setStopwatch(gameStopwatch_);
                field_was_changed_ = true;
            }
        }
        
        HandleInput();
        Render();

        endFrameTime = clock.getElapsedTime(); 
        if (endFrameTime.asMilliseconds() < frameTime)
            sf::sleep(sf::milliseconds(frameTime - endFrameTime.asMilliseconds()));
    }
}

void Game::HandleInput()
{
    //In order to change state under constatnly pressed left mouse button 
    //handling input for this event use isButtonPressed function
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        leftMouseButtonPressed(sf::Mouse::getPosition(window_));
    
    sf::Event event;
    while (window_.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            field_was_changed_ = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::isButtonPressed(sf::Mouse::Right))
                bothMouseButoonPressed({ event.mouseButton.x, event.mouseButton.y });
            else if (event.mouseButton.button == sf::Mouse::Right)
                rightMouseButtonPressed({ event.mouseButton.x, event.mouseButton.y });
            break;
        case sf::Event::MouseButtonReleased:
            field_was_changed_ = true;
            if (event.mouseButton.button == sf::Mouse::Left)
                leftMouseButtonReleased({ event.mouseButton.x, event.mouseButton.y });
            break;
        case sf::Event::Closed:
            window_.close();
            window_is_running_ = false;
            break;
        }    
    }
}

void Game::Render()
{
    //render only on changes
    if (field_was_changed_)
    {
        window_.clear(interface_.backgroundColor());
        window_.draw(interface_);
        for (Tile tile : tiles_)
            window_.draw(tile);
        window_.display();
        //change variable back to default
        field_was_changed_ = false;
    }
}

//A function create B_HEIGHT*B_WIDTH tiles
void Game::createGameField()
{
    //Creates tiles, sets default texture and move each of them for vector of tiles
    for (int i = 0; i < B_HEIGHT; ++i)
    {
        for (int j = 0; j <B_WIDTH; ++j)
        {
            Tile tile{};
            sf::Vector2f position({ static_cast<float>(j * TILE_SIDE_SIZE + B_MARGIN) },
                { static_cast<float>(i * TILE_SIDE_SIZE + B_MARGIN) });

            tile.setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/primary_tile.png"));
            tile.setPosition(position);

            tiles_.emplace_back(tile);
        }
    }
}

void Game::resetGameField()
{
    game_started_ = false;
    game_over_ = false;
    mines_to_find_amount_ = MINE_AMOUNT;
    gameStopwatch_ = 0;
    tile_revealed_amount_ = 0;
    interface_.resetCounters();

    for (int i = 0; i < B_HEIGHT; ++i)
    {
        for (int j = 0; j < B_WIDTH; ++j)
        {
            Tile& tile = tileAt({ j, i });
            tile.resetTile();
            tile.setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/primary_tile.png"));
        }
    }
    field_was_changed_ = true;
}

//Randomly sets mines on field, except 3x3 area around pressed tile
void Game::initializeMine(const sf::Vector2i& startGridCoord)
{
    int mineLeft = MINE_AMOUNT;
    std::random_device engine;
    std::uniform_int_distribution<int> r_row_generator(0,B_HEIGHT-1);
    std::uniform_int_distribution<int> r_columns_generator(0, B_WIDTH-1);

    while (mineLeft > 0)
    {
        sf::Vector2i randomMinePos{ r_columns_generator(engine), r_row_generator(engine) };

        int index = randomMinePos.y * B_WIDTH + randomMinePos.x; //calculate index in order to access to correct tile in vector of tiles
        int initial_index = startGridCoord.y * B_WIDTH + startGridCoord.x;

        //Check if randomly generated tile is in 3x3 area around pressed tile
        bool isAroundFirstTile = (  (index == initial_index) ||
                                    (index == initial_index + 1) ||
                                    (index == initial_index - 1) ||
                                    (index == initial_index + B_WIDTH) ||
                                    (index == initial_index - B_WIDTH) ||
                                    (index == initial_index + 1 + B_WIDTH) ||
                                    (index == initial_index + 1 - B_WIDTH) ||
                                    (index == initial_index - 1 + B_WIDTH) ||
                                    (index == initial_index - 1 - B_WIDTH));

        if (!isAroundFirstTile && !tileAt(randomMinePos).hasMine())
        {
            tileAt(randomMinePos).setMine();
            --mineLeft;
        }  
    }
}

//Reveals tile after a left mouse button click
void Game::revealTile(const sf::Vector2i& tileGridCoord)
{
    if (tileAt(tileGridCoord).state() == Tile::State::pressed)
    {
        tileAt(tileGridCoord).changeTileState(Tile::State::revealed);

        if (!game_started_)
        {
            game_started_ = true;
            initializeMine(tileGridCoord);
            countAmountMineNear();
        }

        //situation when we hited the mine
        if (tileAt(tileGridCoord).hasMine()) 
            mineExploded(tileGridCoord);
        else 
        {
            //if this tile an empty tile
            if (tileAt(tileGridCoord).amountBombNear() == 0)
            {
                ++tile_revealed_amount_;
                tileAt(tileGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/empty_tile.png"));
                revealTilesNear(tileGridCoord);
            }
            else
            {
                ++tile_revealed_amount_;
                std::string tile_path = "resources/tile_" + std::to_string(tileAt(tileGridCoord).amountBombNear()) + ".png";
                tileAt(tileGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource(tile_path));
            }

            //check for a win state
            if (tile_revealed_amount_ == B_WIDTH * B_HEIGHT - MINE_AMOUNT)
            {
                game_started_ = false;
                game_over_ = true;
            }
        }
    } 
}

//Reveal 3x3(if it posible) area around the tile with eventGridCoord coordinates
void Game::revealTilesNear(const sf::Vector2i& eventGridCoord)
{
    //finding a range of cheching to prevent out of range access
    int xStart = std::max(0, eventGridCoord.x - 1);
    int yStart = std::max(0, eventGridCoord.y - 1);
    int xEnd = std::min(eventGridCoord.x + 1, B_WIDTH - 1);
    int yEnd = std::min(eventGridCoord.y + 1, B_HEIGHT - 1);

    for (int yPos = yStart; yPos <= yEnd; ++yPos)
    {
        for (int xPos = xStart; xPos <= xEnd; ++xPos)
        {
            if (tileAt({ xPos, yPos }).state() == Tile::State::primary)
            {
                tileAt({ xPos, yPos }).changeTileState(Tile::State::pressed);
                revealTile({ xPos, yPos });
            }
        }     
    }
}

 void Game::mineExploded(const sf::Vector2i& tileGridCoord)
 {
     game_started_ = false;
     game_over_ = true;
     for (int i = 0; i < B_HEIGHT; ++i)
     {
         for (int j = 0; j < B_WIDTH; ++j)
         {
             if (tileAt({ j,i }).state() == Tile::State::flagged && !tileAt({ j,i }).hasMine())
                 tileAt({ j,i }).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/wrong_mine_tile.png"));

             else if (tileAt({ j,i }).hasMine() && tileAt({ j,i }).state() != Tile::State::flagged)
                 tileAt({ j,i }).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/has_mine_tile.png"));
         }
     }
     tileAt(tileGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/mine_exploded_tile.png"));
 }

 //Finds tiles with mines and increment counter of mines around for not-mine tiles
 void Game::countAmountMineNear()
 {
     for (int i = 0; i < B_HEIGHT; ++i)
     {
         for (int j = 0; j < B_WIDTH; ++j)
         {
             if (tileAt({ j, i }).hasMine())
             {
                 //finding a range of cheching to prevent out of range access
                 int xStart = std::max(0, j - 1);
                 int yStart = std::max(0, i - 1);
                 int xEnd = std::min(j + 1, B_WIDTH - 1);
                 int yEnd = std::min(i + 1, B_HEIGHT - 1);
                 for (int yPos = yStart; yPos <= yEnd; ++yPos)
                 {
                     for (int xPos = xStart; xPos <= xEnd; ++xPos)
                     {
                         if (!tileAt({ xPos, yPos }).hasMine())
                             tileAt({ xPos, yPos }).incrementMineNearCounter();
                     }
                 }
             }
         }
     }
 }

 //Changes state and texture after right mousebuttonpressed event
void Game::rightMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);

    if (!game_over_ && isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/flagged_tile.png"));
            tileAt(eventGridCoord).changeTileState(Tile::State::flagged);
            --mines_to_find_amount_;
        }
        else if (tileAt(eventGridCoord).state() == Tile::State::flagged)
        {
            tileAt(eventGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/primary_tile.png"));
            tileAt(eventGridCoord).changeTileState(Tile::State::primary);
            ++mines_to_find_amount_;
        }
    }
}

//Changes state and texture on tile after left mousebutttonpressed event
void Game::leftMouseButtonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);
  
    //Prevents pressing the field if game is over 
    if (!game_over_ )
    {
        field_was_changed_ = true;
        //if we pressed new tile
        if ( eventGridCoord != previousPressedTileGridCoord_)
        {
            //we set to default state previously pressed tile
            if (isValidGridCoord(previousPressedTileGridCoord_) &&
                tileAt(previousPressedTileGridCoord_).state() != Tile::State::revealed)
            {
                tileAt(previousPressedTileGridCoord_).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/primary_tile.png"));
                tileAt(previousPressedTileGridCoord_).changeTileState(Tile::State::primary);
            }
            //and assign new previous tile
            previousPressedTileGridCoord_ = eventGridCoord;
        }

        //check if tile able to be pressed, and if so, we change its state
        if (isValidGridCoord(eventGridCoord) && tileAt(eventGridCoord).state() == Tile::State::primary)
        {
            tileAt(eventGridCoord).setTexture(ResourceManager<sf::Texture>::Instance().GetResource("resources/empty_tile.png"));
            tileAt(eventGridCoord).changeTileState(Tile::State::pressed);
        }
    }
}

void Game::leftMouseButtonReleased(const sf::Vector2i& eventCoord)
{
    
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);
    if (isValidGridCoord(eventGridCoord) &&
        tileAt(eventGridCoord).state() == Tile::State::pressed)
    {
        revealTile(eventGridCoord);
    }
    else//one tile was pressed, so program have to find it in order to finish sequence: pressed->released
    {
        for (int i = 0; i < B_HEIGHT; ++i)
        {
            for (int j = 0; j < B_WIDTH; ++j)
            {
                if (tileAt({ j,i }).state() == Tile::State::pressed)
                    revealTile({ j, i }); 
            }
        }
    }
    if (interface_.isResetButton(eventCoord))
        resetGameField();
}

void Game::bothMouseButoonPressed(const sf::Vector2i& eventCoord)
{
    sf::Vector2i eventGridCoord = coordToGridCoord(eventCoord);
    if (!game_over_ && isValidGridCoord(eventGridCoord))
    {
        if (tileAt(eventGridCoord).state() == Tile::State::revealed)
        {
            int xStart = std::max(0, eventGridCoord.x - 1);
            int yStart = std::max(0, eventGridCoord.y - 1);
            int xEnd = std::min(eventGridCoord.x + 1, B_WIDTH - 1);
            int yEnd = std::min(eventGridCoord.y + 1, B_HEIGHT - 1);
            int flagged_mine_near{0};
            for (int yPos = yStart; yPos <= yEnd; ++yPos)
            {
                for (int xPos = xStart; xPos <= xEnd; ++xPos)
                {
                    if(tileAt({xPos,yPos}).state() == Tile::State::flagged)
                        ++flagged_mine_near;
                }
            }
            if (flagged_mine_near == tileAt(eventGridCoord).amountBombNear())
                revealTilesNear(eventGridCoord);
        }
    }
}

bool Game::isValidGridCoord(const sf::Vector2i& tileGridCoord)
{
    return tileGridCoord.x >= 0 && tileGridCoord.x < B_WIDTH
        && tileGridCoord.y >= 0 && tileGridCoord.y < B_HEIGHT;
}

//A functions returns a reference to a tile with (x,y) grid coordinates
Tile& Game::tileAt(sf::Vector2i& gridCoord)
{
    return tiles_[(gridCoord.y * size_t(B_WIDTH)) + gridCoord.x];
}

Tile& Game::tileAt(const sf::Vector2i& gridCoord) 
{
    return tiles_[(gridCoord.y * size_t(B_WIDTH)) + gridCoord.x];
}

//A function convert regular coordinates to grid coordinates
sf::Vector2i Game::coordToGridCoord(const sf::Vector2i& coord)
{
    int xCoord = (coord.x - B_MARGIN);
    int yCoord = (coord.y - B_MARGIN);
    if (xCoord < 0)
        xCoord -= TILE_SIDE_SIZE;
    if (yCoord < 0)
        yCoord -= TILE_SIDE_SIZE;

    return{ xCoord / TILE_SIDE_SIZE, yCoord / TILE_SIDE_SIZE };
}
