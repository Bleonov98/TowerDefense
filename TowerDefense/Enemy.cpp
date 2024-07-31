#include "Enemy.h"

void Enemy::InitPath(const std::vector<std::vector<Grid*>> grid)
{
	dirQ.push(std::make_pair(MOVE_UP, grid[13][9]));
	dirQ.push(std::make_pair(MOVE_RIGHT, grid[6][9]));
	dirQ.push(std::make_pair(MOVE_DOWN, grid[6][19]));
	dirQ.push(std::make_pair(MOVE_DOWN, grid[29][19]));
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
	else if (dirQ.empty()) DeleteObject();

	switch (dir)
	{
	case MOVE_RIGHT:
		angle = glm::vec3(-90.0f, 0.0f, 0.0f);
		break;
	case MOVE_LEFT:
		angle = glm::vec3(-90.0f, 0.0f, 180.0f);
		break;
	case MOVE_UP:
		angle = glm::vec3(-90.0f, 0.0f, 90.0f);
		break;
	case MOVE_DOWN:
		angle = glm::vec3(-90.0f, 0.0f,  -90.0f);
		break;
	}
}

void Enemy::Hit(int damage, float slowRate)
{
	this->hp -= damage;
	if (slowRate > 0.0f) {
		this->slowRate = slowRate;
		this->slowTick = 0.0f;
	}
	if (hp <= 0) DeleteObject();
}

void Enemy::SetHP()
{
	float percentHP = static_cast<float>(hp) / static_cast<float>(maxHp);
	glm::vec3 indColour;
	percentHP > 0.65f ? indColour = glm::vec3(0.0f, 1.0f, 0.0f) : (percentHP > 0.3f ? indColour = glm::vec3(0.85f, 0.85f, 0.0f) : indColour = glm::vec3(0.9f, 0.1f, 0.0f));
	indicator.SetColour(indColour);
	indicator.SetPosition(glm::vec3(position.x, position.y + GetSize().z, position.z));
	indicator.SetSize(glm::vec2(indicator.GetSize().first.x * percentHP, indicator.GetSize().first.y));

	indicator.RefreshData();
}

void Enemy::UpgradeEnemy()
{
	maxHp += 175;
	if (maxSpeed < 2.5f) maxSpeed += 0.5f;
	gold += 2;
}
