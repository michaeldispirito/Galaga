#pragma once
#include "Enemy.h"
class Moth : public Enemy
{
public:
    Moth(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect);
};

