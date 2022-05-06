#include "Precompiled.h"
#include "EnemyManager.h"
#include <iostream>

EnemyManager::EnemyManager(const sf::Texture* const texture, const sf::IntRect& beeTextureRect, const sf::IntRect& mothTextureRect, const sf::IntRect& bossTextureRect, const int numberOfEntrancePaths):
	rngEntrancePath(0, numberOfEntrancePaths - 1),
	rngAttack(0,99)
{

	InitializeEnemies(20, &beeEnemies, 2, 10, beeTextureRect, texture);
	InitializeEnemies(16, &mothEnemies, 2, 8, mothTextureRect, texture);
	InitializeEnemies(4, &bossEnemies, 1, 4, bossTextureRect, texture);

	enemyDeathBuffer.loadFromFile("../Music/EnemyDeath.wav");
	enemyDeathSound.setBuffer(enemyDeathBuffer);
	enemyAttackBuffer.loadFromFile("../Music/EnemyAttack.wav");
	enemyAttackSound.setBuffer(enemyAttackBuffer);

}

void EnemyManager::InitializeEnemies(const int reserveSize, std::vector<std::unique_ptr<Enemy>>* enemyVector, const int numRows, const int enemiesPerRow, const sf::IntRect& enemyTextureRect, const sf::Texture* const texture)
{
	enemyVector->reserve(reserveSize);
	for (int i = 0; i < enemyVector->capacity(); i++) {
		int row = (i / (enemyVector->capacity() / numRows));
		int col = (i % enemiesPerRow);
		
		//Kinda cheating here. Can't make a fn pointer to a constructor though... Need to find a better way to do this (Template functions?)
		if (reserveSize == 20) {
			enemyVector->emplace_back(std::make_unique<Enemy>(Bee(row, col, texture, enemyTextureRect)));
		}
		else if (reserveSize == 16) {
			enemyVector->emplace_back(std::make_unique<Enemy>(Moth(row, col, texture, enemyTextureRect)));
		}
		else {
			enemyVector->emplace_back(std::make_unique<Enemy>(Boss(row, col, texture, enemyTextureRect)));
		}
		
	}
}

void EnemyManager::UpdateEnemies(Player& player, int& score, std::vector<Path>& attackPaths)
{
	std::vector<Bullet>& playerBullets = player.GetBullets();
	sf::FloatRect playerRect = player.playerShape.getGlobalBounds();
	sf::FloatRect b1Rect = playerBullets[0].GetBulletShape().getGlobalBounds();
	sf::FloatRect b2Rect = playerBullets[1].GetBulletShape().getGlobalBounds();
	sf::FloatRect* b1 = nullptr;
	sf::FloatRect* b2 = nullptr;

	if (playerBullets[0].IsActive()) {
		b1 = &b1Rect;
	}
	if (playerBullets[1].IsActive()) {
		b2 = &b2Rect;
	}

	UpdateEnemyGroup(&beeEnemies, &beeAttackClock, attackPaths, player, playerBullets, playerRect, b1, b2, &remainingBees, score);
	UpdateEnemyGroup(&mothEnemies, &mothAttackClock, attackPaths, player, playerBullets, playerRect, b1, b2, &remainingMoths, score);
	UpdateEnemyGroup(&bossEnemies, &bossAttackClock, attackPaths, player, playerBullets, playerRect, b1, b2, &remainingBosses, score);

}

void EnemyManager::UpdateEnemyGroup(std::vector<std::unique_ptr<Enemy>>* enemyVector, sf::Clock* enemyAttackClock, std::vector<Path>& attackPaths, Player& player, std::vector<Bullet>& playerBullets, const sf::FloatRect& playerRect, sf::FloatRect* b1, sf::FloatRect* b2, int * remainingEnemies, int& score) {

	for (auto& enemy : *enemyVector) {
		if (enemy->IsEnabled()) {
			Enemy::EnemyState enemyState = enemy->GetState();
			if (enemyState == Enemy::EnemyState::Idle && enemyAttackClock->getElapsedTime().asSeconds() > 2) {
				SetEnemyAction(enemy.get(), attackPaths);
				enemyAttackClock->restart();

			}
			else if (enemyState == Enemy::EnemyState::FollowingPath) {
				if (!player.IsInvincible() && enemy->CollidedWithPlayer(playerRect)) {
					player.Destroy();
				}
			}
			enemy->Move();

			int index = enemy->CollidedWithBullet(b1, b2);
			if (index >= 0) {
				player.ResetBullet(playerBullets[index], index);
				if (index == 0) {
					b1 = nullptr;
				}
				else {
					b2 = nullptr;
				}
				enemy->Disable();
				enemyDeathSound.play();
				score += 10;
				*remainingEnemies -= 1;
			}

		}
	}
}

void EnemyManager::SetupRound(std::vector<Path>& entrancePaths)
{
	remainingEnemyTypes.emplace_back(0);
	remainingEnemyTypes.emplace_back(1);
	remainingEnemyTypes.emplace_back(2);

	ResetEnemyCounts();

	nextUnusedBee = 0;
	nextUnusedMoth = 0;
	nextUnusedBoss = 0;

	int curEPIndex = 0;
	while (remainingBees > 0 || remainingMoths > 0 || remainingBosses > 0) {
		Path* ep = &entrancePaths[rngEntrancePath(mt)];

		for (int i = 0; i < 2; i++) {
			int startingIndex = i == 0 ? 0 : 4;
			std::uniform_int_distribution<int> rngEnemyType(0, remainingEnemyTypes.size() - 1);

			int enemyIndex = rngEnemyType(mt);

			switch (remainingEnemyTypes[enemyIndex]) {
			case 0: //bee
				SetEnemyPath(ep, &nextUnusedBee, &remainingBees, &beeEnemies, enemyIndex, curEPIndex, startingIndex);
				break;

			case 1: //moth
				SetEnemyPath(ep, &nextUnusedMoth, &remainingMoths, &mothEnemies, enemyIndex, curEPIndex, startingIndex);
				break;

			case 2: //boss
				SetEnemyPath(ep, &nextUnusedBoss, &remainingBosses, &bossEnemies, enemyIndex, curEPIndex, startingIndex);
				break;
			}
		}
		curEPIndex++;
	}

	curPathIndex = 0;
	curEnemyIndex = 0;
	holdNextPath = false;
	ResetEnemyCounts();
}

bool EnemyManager::SendNextEnemy()
{
	bool sendEnemy = false;
	if (holdNextPath) {
		if (entranceClock.getElapsedTime().asSeconds() > 2) {
			sendEnemy = true;
		}
	}
	else {
		if (entranceClock.getElapsedTime().asSeconds() > 0.2f) {
			sendEnemy = true;
		}
	}
	if (sendEnemy) {
		nextEnemyArray[curPathIndex][curEnemyIndex]->SetState(Enemy::EnemyState::ReadyForEntrance);

		curEnemyIndex++;
		if (curEnemyIndex > 7) {
			curPathIndex++;
			curEnemyIndex = 0;
			holdNextPath = true;
		}
		else {
			holdNextPath = false;
		}
		entranceClock.restart();
	}
	return (curPathIndex == 5);
}

bool EnemyManager::AllEnemiesDefeated()
{
	int enemyCount = remainingBees + remainingMoths + remainingBosses;
	if (enemyCount == 0) {
		attackPercent = attackPercent < 100 ? attackPercent + 5 : 100;
		return true;
	}
	return false;
}

void EnemyManager::Render(sf::RenderWindow& window)
{
	for (auto& bee: beeEnemies) {
		if (bee->IsEnabled()) {
			window.draw(bee->GetShape());
		}
	}

	for (auto& moth : mothEnemies) {
		if (moth->IsEnabled()) {
			window.draw(moth->GetShape());
		}
	}

	for (auto& boss : bossEnemies) {
		if (boss->IsEnabled()) {
			window.draw(boss->GetShape());
		}
	}
}

void EnemyManager::SetEnemyPath(Path* ep, int* nextUnusedIndex, int* remainingEnemiesOfTypeX, std::vector<std::unique_ptr<Enemy>>* enemyVector, const int enemyIndex, const int curEPIndex, const int startingIndex)
{
	for (int i = 0; i < 4; i++) {
		Enemy* enemy = (*enemyVector)[*nextUnusedIndex].get();
		enemy->SetEntrancePath(ep);
		nextEnemyArray[curEPIndex][i + startingIndex] = enemy;
		(*nextUnusedIndex)++;
			
	}
	*remainingEnemiesOfTypeX -= 4;
	if (*remainingEnemiesOfTypeX == 0) {
		remainingEnemyTypes.erase(remainingEnemyTypes.begin() + enemyIndex);
	}
}

void EnemyManager::SetEnemyAction(Enemy* enemy, std::vector<Path>& attackPaths)
{
	std::uniform_int_distribution<int> rngPath(0, attackPaths.size() - 1);
	int diveDown = rngAttack(mt);
	if (diveDown < attackPercent) {
		Path* path = &(attackPaths[rngPath(mt)]);
		if (!path->InUse()) {
			path->SetInUse();
			enemy->SetAttackPath(path);
			enemyAttackSound.play();
		}
		
	}
}

void EnemyManager::ResetEnemyCounts()
{
	remainingBees = beeEnemies.capacity();
	remainingMoths = mothEnemies.capacity();
	remainingBosses = bossEnemies.capacity();
}
