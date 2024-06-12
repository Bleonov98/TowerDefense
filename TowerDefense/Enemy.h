#ifndef ENEMY_H
#define ENEMY_H

#include <queue>

#include "GameObject.h"
#include "Indicator.h"
#include "Grid.h"

enum MoveDir {
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_UP,
	MOVE_DOWN
};

class Enemy : public GameObject
{
public:

	Enemy(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle)
	{
		this->model = model;

		RefreshModel();

		this->hp = this->maxHp;
		this->speed = this->maxSpeed;
		indicator.AddTexture(ResourceManager::GetTexture("indicator"));
	}

	void InitPath(const std::vector<std::vector<Grid*>> grid);

	int GetHP() { return this->hp; }
	int GetSpeed() { return this->speed; }
	int GetGold() { return this->gold; }

	void Move(const float dt);
	void CheckPoint();
	void Hit(int damage, float slowRate);

	void SetIndicator(Indicator indicator) { this->indicator = indicator; }
	Indicator& GetIndicator() { return this->indicator; }
	void ShowHP(glm::mat4 projection, glm::mat4 view, bool menu);

	void UpgradeEnemy();

protected:

	inline static float maxSpeed = 1.0f;
	inline static int maxHp = 100, gold = 10;
	
	float slowRate = 0.0f, slowTick = 0.0f, slowDuration = 2.0f, speed;
	int hp;

	Indicator indicator;
	MoveDir dir = MOVE_RIGHT;
	std::queue<pair<MoveDir, Grid*>> dirQ;
};

#endif // !ENEMY_H