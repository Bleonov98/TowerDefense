#ifndef ENEMY_H
#define ENEMY_H

#include <queue>

#include "GameObject.h"
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

	Enemy(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {
		this->model = model;

		RefreshModel();
	}

	void InitPath(const std::vector<std::vector<Grid*>> grid);

	int GetHP() { return this->hp; }
	int GetSpeed() { return this->speed; }
	int GetGold() { return this->gold; }

	void Move(const float dt);
	void CheckPoint();
	int Hit(const int damage);

	void UpgradeEnemy();

protected:

	inline static int speed = 1, hp = 100, gold = 10;

	MoveDir dir = MOVE_RIGHT;
	std::queue<pair<MoveDir, Grid*>> dirQ;

};

#endif // !ENEMY_H