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

	// background texture
	ResourceManager::GetShader("indShader").Use();
	ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("indShader").SetMatrix4("view", view);
	ResourceManager::GetShader("indShader").SetBool("menu", menu);
	ResourceManager::GetShader("indShader").SetBool("isImage", true);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, indicator.position);

	ResourceManager::GetShader("testShader").SetMatrix4("model", modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	indicator.texture.Bind();

	glBindVertexArray(indicator.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// colour instance
	ResourceManager::GetShader("indShader").Use();
	ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("indShader").SetMatrix4("view", view);

	ResourceManager::GetShader("indShader").SetBool("menu", menu);
	ResourceManager::GetShader("indShader").SetBool("isImage", false);
	ResourceManager::GetShader("indShader").SetVector3f("spriteColour", indColour);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, indicator.position);
}

void Enemy::UpgradeEnemy()
{
	maxHp += 150;
	if (maxSpeed < 2.5f) maxSpeed += 0.5f;
	gold += 5;
}

Indicator::Indicator(glm::vec3 position, glm::vec2 size)
{
	this->position = position;
	this->indSize = this->size = size;

	// halves
	float x, y;
	x = size.x / 2.0f;
	y = size.y / 2.0f;

	// indicator shapes
	float vertices[] = {
		-x, -y,  0, 0.0f, 1.0f,
		-x,  y,  0, 1.0f, 0.0f,
		 x, -y,  0, 0.0f, 0.0f,

		 x, -y,  0, 0.0f, 1.0f,
		-x,  y,  0, 1.0f, 1.0f,
		 x,  y,  0, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// textures
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
