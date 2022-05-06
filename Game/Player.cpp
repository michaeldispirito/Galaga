#include "Precompiled.h"
#include "Player.h"

Player::Player(const int width, const int height, const sf::Texture* const texture, const sf::IntRect& textureRect, const sf::IntRect& bulletTextureRect):
	playerShape(sf::Vector2f(16, 16)),
	GAME_WIDTH(width),
	GAME_HEIGHT(height),
	bulletVector(MAX_BULLETS, Bullet(texture, bulletTextureRect))
{
	playerShape.setOrigin(sf::Vector2f(7.5f, 7.5f));
	playerShape.setTexture(texture); 
	playerShape.setTextureRect(textureRect);
	playerShape.setScale(sf::Vector2f(2, 2));

	playerShape.setPosition(sf::Vector2f((width - 1) / 2, height - 50));

	for (int i = 0; i < MAX_BULLETS; i++) {
		availableBullets.push(i);
	}

	bulletBuffer.loadFromFile("../Music/BulletFire.wav");
	playerDeathBuffer.loadFromFile("../Music/PlayerDeath.wav");

	bulletSound.setBuffer(bulletBuffer);
	playerDeathSound.setBuffer(playerDeathBuffer);
}

void Player::CheckKeyboardInput()
{
	if (!isAlive) {
		if (clockDeathReset.getElapsedTime().asSeconds() > 3) {
			Respawn();
		}
	}
	else {
		CheckInvincibility();
		MovePlayer();
		FireBullet();
	}
}

void Player::UpdateBullets()
{
	if (availableBullets.size() != MAX_BULLETS) {
		int index = 0;
		for (Bullet& b : bulletVector) {
			if (b.IsActive()) {
				b.move();

				if (b.GetPosition().y < 0) {
					ResetBullet(b, index);
				}
			}
			index++;
		}
	}
}

void Player::StopShooting()
{
	isShooting = false;
}

void Player::ResetBullet(Bullet& b, const int index)
{
	b.Disable();
	b.MoveToDefaultPosition();
	availableBullets.emplace(index);
}

void Player::Destroy()
{
	lives -= 1;
	isAlive = false; 
	playerShape.setPosition(sf::Vector2f(-25, -25));
	playerDeathSound.play();
	clockDeathReset.restart();

}

void Player::Respawn()
{
	playerShape.setPosition(sf::Vector2f((GAME_WIDTH - 1) / 2, GAME_HEIGHT - 50));
	playerShape.setFillColor(sf::Color::Yellow);
	isAlive = true;
	isInvincible = true;
	clockInvincibilty.restart();
}

std::vector<Bullet>& Player::GetBullets()
{
	return bulletVector;
}

int Player::GetLives() const
{
	return lives;
}

bool Player::IsInvincible() const
{
	return isInvincible;
}

void Player::CheckInvincibility()
{
	if (clockInvincibilty.getElapsedTime().asSeconds() > 2) {
		playerShape.setFillColor(sf::Color::White);
		isInvincible = false;
	}
}

void Player::MovePlayer()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		if (playerShape.getPosition().x - playerShape.getGlobalBounds().width/2 > 0) {
			playerShape.move(-SPEED, 0);
		}

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		if (playerShape.getPosition().x + playerShape.getGlobalBounds().width < GAME_WIDTH) {
			playerShape.move(SPEED, 0);
		}

	}
}

void Player::FireBullet()
{
  	if (availableBullets.size() == 0 || clockShooting.getElapsedTime().asMilliseconds() < GetMinFireDelay()) {
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
		Bullet& newBullet = bulletVector.at(availableBullets.front());
		newBullet.SetPosition(sf::Vector2f(playerShape.getPosition().x, playerShape.getPosition().y));
		newBullet.Enable();
		if (bulletSound.getStatus() != 3) {
			bulletSound.play();
		}
		
		availableBullets.pop();
		clockShooting.restart();
		isShooting = true;
	}
}

sf::Int32 Player::GetMinFireDelay()
{
	if (isShooting) {
		return 400;
	}
	else {
		return 0;
	}
}

void Player::Render(sf::RenderWindow& window)
{
	if (isAlive) {
		window.draw(playerShape);
	}
	for (const Bullet& b : bulletVector) {
		if (b.IsActive()) {
			window.draw(b.GetBulletShape());
		}
	}
}
