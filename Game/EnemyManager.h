#pragma once
#include <vector>
#include <queue>
#include "Enemy.h"
#include "Bee.h"
#include "Moth.h"
#include "Boss.h"
#include <random>
#include <time.h>
#include "Player.h"


class EnemyManager
{
public:
	EnemyManager(const sf::Texture* const texture, const sf::IntRect& beeTextureRect, const sf::IntRect& mothTextureRect, const sf::IntRect& bossTextureRect, const int numberOfEntrancePaths);
	
	void UpdateEnemies(Player& player, int& score, std::vector<Path>& attackPaths);
	void SetupRound(std::vector<Path>& entrancePaths);
	bool SendNextEnemy();
	bool AllEnemiesDefeated();


	void Render(sf::RenderWindow& window);



private:
	std::vector<std::unique_ptr<Enemy>> beeEnemies;
	std::vector<std::unique_ptr<Enemy>> mothEnemies;
	std::vector<std::unique_ptr<Enemy>> bossEnemies;

	int remainingBees = 0;
	int remainingMoths = 0;
	int remainingBosses = 0;

	int nextUnusedBee = 0;
	int nextUnusedMoth = 0;
	int nextUnusedBoss = 0;

	float attackPercent = 50;
	std::mt19937 mt{std::time(nullptr)}; 
	std::uniform_int_distribution<int> rngEntrancePath;
	std::uniform_int_distribution<int> rngAttack;
	std::vector<int> remainingEnemyTypes;
	sf::Clock beeAttackClock;
	sf::Clock mothAttackClock;
	sf::Clock bossAttackClock;

	Enemy* nextEnemyArray[5][8] = {0};
	int curPathIndex = 0;
	int curEnemyIndex = 0;
	sf::Clock entranceClock;
	bool holdNextPath = false;

	sf::SoundBuffer enemyDeathBuffer;
	sf::Sound enemyDeathSound;
	sf::SoundBuffer enemyAttackBuffer;
	sf::Sound enemyAttackSound;

	void SetEnemyPath(Path* ep, int* nextUnusedIndex, int* remainingEnemiesOfTypeX, std::vector<std::unique_ptr<Enemy>>* enemyVector, const int enemyIndex, const int curEPIndex, const int startingIndex);
	void SetEnemyAction(Enemy* enemy, std::vector<Path>& attackPaths);
	void ResetEnemyCounts();

	void InitializeEnemies(const int reserveSize, std::vector<std::unique_ptr<Enemy>>* enemyVector, const int numRows, const int enemiesPerRow, const sf::IntRect& enemyTextureRect, const sf::Texture* const texture);
	void UpdateEnemyGroup(std::vector<std::unique_ptr<Enemy>>* enemyVector, sf::Clock* enemyAttackClock, std::vector<Path>& attackPaths, Player& player, std::vector<Bullet>& playerBullets, const sf::FloatRect& playerRect, sf::FloatRect* b1, sf::FloatRect* b2, int* remainingEnemies, int& score);
};

