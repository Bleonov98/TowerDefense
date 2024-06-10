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

void Enemy::Hit(int damage, float slowRate)
{
	this->hp -= damage;
	if (slowRate > 0.0f) {
		this->slowRate = slowRate;
		this->slowTick = 0.0f;
	}
	if (hp <= 0) DeleteObject();
}

void Enemy::ShowHP(glm::mat4 projection, glm::mat4 view, bool menu)
{
	float percentHP = hp / maxHp;
	glm::vec3 indColour;
	percentHP > 0.5f ? indColour = glm::vec3(0.0f, 1.0f, 0.0f) : (percentHP > 0.25f ? indColour = glm::vec3(0.5f, 0.5f, 0.0f) : indColour = glm::vec3(0.9f, 0.1f, 0.0f));

	ResourceManager::GetShader("indShader").Use();
	ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("indShader").SetMatrix4("view", view);
	ResourceManager::GetShader("indShader").SetBool("menu", menu);
	ResourceManager::GetShader("indShader").SetVector3f("spriteColor", indColour);

	//// hp shapes
	//float vertices[] = {
	//	-x, 0, -z,
	//	 x, 0, -z,
	//	 x, 0,  z,
	//	 x, 0,  z,
	//	-x, 0,  z,
	//	-x, 0, -z
	//};
}

void Enemy::UpgradeEnemy()
{
	maxHp += 150;
	if (maxSpeed < 2.5f) maxSpeed += 0.5f;
	gold += 5;
}