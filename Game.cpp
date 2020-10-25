#include "Game.h"

Game::Game(): running_{false}, window_(sf::VideoMode(1200, 800), "Minesweeper")
{

}

void Game::Run()
{
    running_ = true;
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
            window_.close();
    }
}

void Game::Update()
{
}

void Game::Render()
{
    window_.clear();
    window_.display();
}
