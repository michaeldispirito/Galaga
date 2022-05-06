#include "Precompiled.h"
#include "Boss.h"

Boss::Boss(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect) :
	Enemy(row, column, 1, 3, texture, textureRect)
{
}
