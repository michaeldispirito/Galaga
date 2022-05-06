#pragma once
#include "Enemy.h"
class Boss : public Enemy
{

public:
    Boss(const int row, const int column, const sf::Texture* const texture, const sf::IntRect& textureRect);
};

