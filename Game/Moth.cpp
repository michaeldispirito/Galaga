#include "Precompiled.h"
#include "Moth.h"

Moth::Moth(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect) :
	Enemy(row, column, 2, 1, texture, textureRect)
{
}