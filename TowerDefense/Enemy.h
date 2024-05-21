#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
class Enemy : public GameObject
{
public:

	Enemy(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {
		this->model = model;

		RefreshModel();
	}

	int GetHP() { return this->hp; }
	int GetSpeed() { return this->speed; }
	int GetGold() { return this->gold; }

	void Move(float dt);
	void Hit(int damage);

	void UpgradeEnemy();

protected:

	int speed = 100, hp = 100, gold = 10;

};

#endif // !ENEMY_H