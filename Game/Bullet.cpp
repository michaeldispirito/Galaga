#include "Precompiled.h"
#include "Bullet.h"

Bullet::Bullet(const sf::Texture* const texture, const sf::IntRect& textureRect):
    bulletShape(sf::Vector2f(8, 8))
{
    bulletShape.setOrigin(sf::Vector2f(4, 6));
    bulletShape.setTexture(texture);
    bulletShape.setTextureRect(textureRect);
    bulletShape.setScale(2, 2);

    this->position = (sf::Vector2f(-20, -20));
    bulletShape.setPosition(position);
}

Bullet& Bullet::operator=(const Bullet& other)
{
    position = other.position;
    bulletShape = other.bulletShape;
    return *this;
}

void Bullet::move()
{
    position.y -= SPEED;
    bulletShape.setPosition(position);
}

sf::Vector2f Bullet::GetPosition() const
{
    return position;
}

void Bullet::SetPosition(sf::Vector2f&& newPosition)
{
    this->position = newPosition;
    bulletShape.setPosition(newPosition);
}

void Bullet::MoveToDefaultPosition()
{
    SetPosition(sf::Vector2f(-20, -20));
}

sf::RectangleShape Bullet::GetBulletShape() const
{
    return bulletShape;
}

bool Bullet::IsActive() const
{
    return enabled;
}

void Bullet::Enable()
{
    enabled = true;
}

void Bullet::Disable()
{
    enabled = false;
}
