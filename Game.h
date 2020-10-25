#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	void Run();
	void HandleInput();
	void Update();
	void Render();

private:
	sf::RenderWindow window_;
	bool running_;
};

