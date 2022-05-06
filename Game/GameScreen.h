#pragma once
#include "SFML/Graphics.hpp"

class GameScreen
{
public:

	GameScreen()
	{
		window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "GALAGA", sf::Style::Close);
		SetupWindowSplit();
	}

	int GetWidth() const;
	int GetHeight() const;
	int GetGameWidth() const;
	int GetStatsWidth() const;

	void Render();
	sf::RenderWindow window;
private:
	int const SCREEN_WIDTH = 600; //NOTE: 0,0 is top left & max values to draw on will be W-1 and H-1
	int const SCREEN_HEIGHT = 500;
	int const GAME_SCREEN_WIDTH = SCREEN_WIDTH * 0.75;
	int const STATS_SCREEN_WIDTH = SCREEN_WIDTH - GAME_SCREEN_WIDTH;

	sf::Vertex windowSplitLine[2];


	void SetupWindowSplit();


};

