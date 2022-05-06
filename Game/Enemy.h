#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Path.h"
#include "Bullet.h"
#include "Player.h"

/*
Reference: 
	- Parts of FollowPath use: https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c
*/


class Enemy
{
public:
	enum class EnemyState {
		Idle,
		StartingPath,
		FollowingPath,
		ReturningToIdle,
		Disabled,
		ReadyForEntrance
	};

	Enemy(const int row, const int col, const int rowOffset, const int colOffset, const sf::Texture* const texture, const sf::IntRect& textureRect);

	void Disable();
	void SetState(EnemyState newState);
	bool IsEnabled() const;
	EnemyState GetState() const;

	void Move();

	void SetEntrancePath(Path* const path);
	void SetAttackPath(Path* const path);

	const sf::RectangleShape& GetShape() const;
	int CollidedWithBullet(sf::FloatRect* const b1Rect, sf::FloatRect* const b2Rect) const;
	bool CollidedWithPlayer(const sf::FloatRect& playerRect) const;

	 
protected:
	sf::RectangleShape enemyShape;
	EnemyState currentState = EnemyState::Disabled;

	const float SPEED = 220;
	float distance = 0;
	
	int row;          // per enemy type. So each new enemy type restarts at row 0
	int column;


	sf::Clock clock;
	Path* curPath = nullptr;
	int curWayPointIndex = 0;
	Path* nextPath = nullptr;

	WayPoint home;
	WayPoint startingPoint{ 0, 0 };

	float getPt(const float n1, const float n2, const float perc) const;

	void ResetPathVariables(EnemyState newState);
	void SetupHomePath();
	void GoToIdle();
	void FollowPath();
	void MoveToStartOfPath();

	sf::Vector2f CalculateNewPosition(const WayPoint& destinationWP) const;

};

