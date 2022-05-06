#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <vector>
#include "Bullet.h"
#include <queue>

class Player
{
public:
	Player(const int width, const int height, const sf::Texture * const texture, const sf::IntRect& textureRect, const sf::IntRect& bulletTextureRect);

	void Render(sf::RenderWindow& window);

	void CheckKeyboardInput();
	void UpdateBullets();
	void StopShooting();
	void ResetBullet(Bullet& b, const int index);

	void Destroy();
	void Respawn();

	std::vector<Bullet>& GetBullets();
	int GetLives() const;
	bool IsInvincible() const;

	sf::RectangleShape playerShape;

private:
	int lives = 3;
	bool isAlive = true;
	bool isInvincible = false;
	sf::Clock clockDeathReset;
	sf::Clock clockInvincibilty;

	const float SPEED = 0.05f;
	const int MAX_BULLETS = 2;
	const int GAME_WIDTH;
	const int GAME_HEIGHT;

	std::vector<Bullet> bulletVector;
	std::queue<int> availableBullets; 
	sf::Clock clockShooting;
	bool isShooting = false;

	void CheckInvincibility();
	void MovePlayer();
	void FireBullet();
	sf::Int32 GetMinFireDelay();

	sf::SoundBuffer bulletBuffer;
	sf::Sound bulletSound;
	sf::SoundBuffer playerDeathBuffer;
	sf::Sound playerDeathSound;

};

