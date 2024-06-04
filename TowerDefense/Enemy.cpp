#include "Enemy.h"

void Enemy::InitPath(const std::vector<std::vector<Grid*>> grid)
{
	dirQ.push(std::make_pair(MOVE_UP, grid[13][9]));
	dirQ.push(std::make_pair(MOVE_RIGHT, grid[6][9]));
	dirQ.push(std::make_pair(MOVE_DOWN, grid[6][19]));
}

void Enemy::Move(const float dt)
{
	slowTick += dt;
	float newSpeed = speed * (1.0f - slowRate);

	if (dir == MOVE_RIGHT) this->position.x += newSpeed * dt;
	else if (dir == MOVE_LEFT) this->position.x -= newSpeed * dt;
	else if (dir == MOVE_UP) this->position.z -= newSpeed * dt;
	else if (dir == MOVE_DOWN) this->position.z += newSpeed * dt;

	if (slowTick >= slowDuration) slowRate = 0.0f;
}

void Enemy::CheckPoint()
{
	if (!dirQ.empty() && dirQ.front().second->CenterCollision(position)) {
		dir = dirQ.front().first;
		dirQ.pop();
	}
}

void Enemy::Hit(const int damage, float slowRate)
{
	this->hp -= damage;
	this->slowRate = slowRate;
	this->slowTick = 0.0f;
	if (hp <= 0) DeleteObject();
}

void Enemy::UpgradeEnemy()
{
	hp += 150;
	if (speed < 2.5f) speed += 0.5f;
	gold += 5;
}
