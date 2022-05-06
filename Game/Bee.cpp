#include "Precompiled.h"
#include "Bee.h"

Bee::Bee(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect) :
	Enemy(row, column, 4, 0, texture, textureRect)
{
} 
