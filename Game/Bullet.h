#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"


class Bullet
{
public:
	Bullet(const sf::Texture* const texture, const sf::IntRect& textureRect);
	

	Bullet& operator=(const Bullet& other);

	void move();

	sf::Vector2f GetPosition() const;
	void SetPosition(sf::Vector2f&& newPosition);

	void MoveToDefaultPosition();
	sf::RectangleShape GetBulletShape() const;

	bool IsActive() const;
	void Enable();
	void Disable();
 

private:
	const float SPEED = 0.13f;
	bool enabled = false;
	sf::Vector2f position;
	sf::RectangleShape bulletShape;


};

