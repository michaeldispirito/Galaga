#pragma once
#include "Enemy.h"
class Bee : public Enemy
{
public:
	Bee(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect);
};

