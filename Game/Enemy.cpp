#include "Precompiled.h"
#include "Enemy.h"


Enemy::Enemy(const int row, const int col, const int rowOffset, const int colOffset, const sf::Texture* const texture, const sf::IntRect& textureRect) :
	row(row),
	column(col),
	home(64 + (36 * (col + colOffset)), 25 + (36 * (row + rowOffset)))
{
	enemyShape.setSize(sf::Vector2f(16, 16));
	enemyShape.setOrigin(sf::Vector2f(8, 8));
	enemyShape.setTexture(texture);
	enemyShape.setTextureRect(textureRect);
	enemyShape.setPosition(sf::Vector2f(-50, -50));
	enemyShape.setScale(2, 2);

}

void Enemy::Disable()
{
	ResetPathVariables(EnemyState::Disabled);
	enemyShape.setPosition(sf::Vector2f(-25, -25));
}

void Enemy::SetState(EnemyState newState)
{
	currentState = newState;
}

bool Enemy::IsEnabled() const
{
	return currentState != EnemyState::Disabled;
}

Enemy::EnemyState Enemy::GetState() const
{
	return currentState;
}

const sf::RectangleShape& Enemy::GetShape() const
{
	return enemyShape;
}

void Enemy::Move()
{
	switch (currentState) {
	case EnemyState::Idle:
		break;
	case EnemyState::ReadyForEntrance:
		clock.restart();
		currentState = EnemyState::FollowingPath;
		break;
	case EnemyState::StartingPath:
		MoveToStartOfPath();
		break;
	case EnemyState::FollowingPath:
		FollowPath();
		break;
	case EnemyState::ReturningToIdle:
		GoToIdle();
		break;
	}
}

void Enemy::FollowPath()
{
	WayPoint* curWayPoint = curPath->GetWayPoint(curWayPointIndex);
	WayPoint* nextWayPoint = curPath->GetWayPoint(curWayPointIndex + 1);

	distance += SPEED * clock.restart().asSeconds();
		while (distance > nextWayPoint->totalDistance) {
			curWayPointIndex++;
			if (curWayPointIndex > curPath->GetLength() - 2) {
				ResetPathVariables(EnemyState::ReturningToIdle);
				SetupHomePath();
				return;
			}
			curWayPoint = curPath->GetWayPoint(curWayPointIndex);
			nextWayPoint = curPath->GetWayPoint(curWayPointIndex + 1);
		}

		float unitDist = (distance - curWayPoint->totalDistance) / curWayPoint->distanceToNext; //Percentage of distance to next waypoint covered by enemy

		//Get point for a Bezier curve (from stack overflow)
		float xa = getPt(curWayPoint->x, curWayPoint->controlPointX, unitDist);
		float ya = getPt(curWayPoint->y, curWayPoint->controlPointY, unitDist);
		float xb = getPt(curWayPoint->controlPointX, nextWayPoint->x, unitDist);
		float yb = getPt(curWayPoint->controlPointY, nextWayPoint->y, unitDist);
		float newX = getPt(xa, xb, unitDist);
		float newY = getPt(ya, yb, unitDist);

		enemyShape.setPosition(sf::Vector2f(newX, newY));
}

void Enemy::MoveToStartOfPath()
{
	WayPoint* startOfPath = curPath->GetWayPoint(0);
	distance += SPEED * clock.restart().asSeconds();
	while (distance > startOfPath->totalDistance) {
		currentState = EnemyState::FollowingPath;
		distance = 0;
		curPath->GetWayPoint(0)->totalDistance = 0;
		return;
	}

	enemyShape.setPosition(CalculateNewPosition(*startOfPath));
}

void Enemy::GoToIdle()
{
	distance += SPEED * clock.restart().asSeconds();
	while (distance > home.totalDistance) {
		currentState = EnemyState::Idle;
		distance = 0;
		return;
	}

	enemyShape.setPosition(CalculateNewPosition(home));
}

sf::Vector2f Enemy::CalculateNewPosition(const WayPoint& destinationWP) const
{
	float unitDist = (distance - startingPoint.totalDistance) / startingPoint.distanceToNext;
	float newX = (destinationWP.x - startingPoint.x) * unitDist + startingPoint.x;
	float newY = (destinationWP.y - startingPoint.y) * unitDist + startingPoint.y;

	return sf::Vector2f(newX, newY);
}

void Enemy::ResetPathVariables(EnemyState newState)
{
	if (curPath != nullptr) {
		curPath->SetAvailable();
	}
	curPath = nullptr;
	curWayPointIndex = 0;
	currentState = newState;
	distance = 0;
}

void Enemy::SetupHomePath()
{
	startingPoint.x = enemyShape.getPosition().x;
	startingPoint.y = enemyShape.getPosition().y;
	startingPoint.addNext(&home, 0, WayPoint::CurveDirection::None);
}

void Enemy::SetEntrancePath(Path* const path)
{
	curPath = path;
	curWayPointIndex = 0;
	distance = 0;
	enemyShape.setPosition(sf::Vector2f(path->GetWayPoint(0)->x, path->GetWayPoint(0)->y));
	currentState = EnemyState::Disabled;
}

void Enemy::SetAttackPath(Path* const path)
{
	curPath = path;
	curWayPointIndex = 0;
	distance = 0;

	startingPoint.x = enemyShape.getPosition().x;
	startingPoint.y = enemyShape.getPosition().y;
	startingPoint.addNext(path->GetWayPoint(0), 0, WayPoint::CurveDirection::None);
	currentState = EnemyState::StartingPath;
	clock.restart();
}

int Enemy::CollidedWithBullet(sf::FloatRect* const b1Rect, sf::FloatRect* const b2Rect) const
{
	if (b1Rect == nullptr && b2Rect == nullptr) {
		return -1;
	}

	sf::FloatRect enemyRect = enemyShape.getGlobalBounds();
	if (b1Rect != nullptr) {
		if (enemyRect.intersects(*b1Rect)) {
			return 0;
		}
	}
	if (b2Rect != nullptr) {
		if (enemyRect.intersects(*b2Rect)) {
			return 1;
		}
	}
	return -1;
}

bool Enemy::CollidedWithPlayer(const sf::FloatRect& playerRect) const
{
	return enemyShape.getGlobalBounds().intersects(playerRect);
}

float Enemy::getPt(const float n1, const float n2, const float perc) const
{
	float diff = n2 - n1;
	return n1 + (diff * perc);
}
