#include "Enemy.h"

void Enemy::InitPath(const std::vector<std::vector<Grid*>> grid)
{
	//dirQ.push(std::make_pair(MOVE_RIGHT, grid[11][0]));
	//dirQ.push(std::make_pair(MOVE_UP, grid[11][11]));
	//dirQ.push(std::make_pair(MOVE_RIGHT, grid[6][11]));
	//dirQ.push(std::make_pair(MOVE_DOWN, grid[6][19]));
	//dirQ.push(std::make_pair(MOVE_RIGHT, grid[11][19]));
	//dirQ.push(std::make_pair(MOVE_DOWN, grid[11][25]));
	//dirQ.push(std::make_pair(MOVE_LEFT, grid[19][25]));
	//dirQ.push(std::make_pair(MOVE_DOWN, grid[19][19]));
	//dirQ.push(std::make_pair(MOVE_LEFT, grid[24][19]));
	//dirQ.push(std::make_pair(MOVE_UP, grid[24][11]));
	//dirQ.push(std::make_pair(MOVE_LEFT, grid[19][11]));
	//dirQ.push(std::make_pair(MOVE_LEFT, grid[19][0]));
}

void Enemy::Move(const float dt)
{
	if (dir == MOVE_RIGHT) this->position.x += speed * dt;
	else if (dir == MOVE_LEFT) this->position.x -= speed * dt;
	else if (dir == MOVE_UP) this->position.z -= speed * dt;
	else if (dir == MOVE_DOWN) this->position.z += speed * dt;
}

void Enemy::CheckPoint()
{
	if (!dirQ.empty() && dirQ.front().second->CenterCollision(position)) {
		dir = dirQ.front().first;
		dirQ.pop();
	}
}

int Enemy::Hit(const int damage)
{
	this->hp -= damage;
	if (hp <= 0) {
		DeleteObject();
		return gold;
	}

	return 0;
}

void Enemy::UpgradeEnemy()
{
	hp += 150;
	if (speed < 2.5f) speed += 0.5f;
	gold += 5;
}
