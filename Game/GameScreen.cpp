#include "Precompiled.h"
#include "GameScreen.h"

int GameScreen::GetWidth() const
{
	return SCREEN_WIDTH;
}

int GameScreen::GetHeight() const
{
	return SCREEN_HEIGHT;
}

int GameScreen::GetGameWidth() const
{
	return GAME_SCREEN_WIDTH;
}

int GameScreen::GetStatsWidth() const
{
	return STATS_SCREEN_WIDTH;
}

void GameScreen::Render()
{
	window.clear();
	window.draw(windowSplitLine, 2, sf::Lines);
}

void GameScreen::SetupWindowSplit()
{
	windowSplitLine[0].position = sf::Vector2f(GAME_SCREEN_WIDTH - 1, 0);
	windowSplitLine[0].color = sf::Color::Red;
	windowSplitLine[1].position = sf::Vector2f(GAME_SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	windowSplitLine[1].color = sf::Color::White;
}
