#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include "Enemy.h"

enum ProjectileType {
	ARROW_P,
	FIREBALL_P,
	ICEBALL_P
};

class Projectile : public GameObject
{
public:

	Projectile(glm::vec3 position, Enemy* target, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {
		this->target = target;
	};


	float GetSpeed() { return projSpeed; }
	void SetSpeed(float speed) { this->projSpeed = speed; }

	ProjectileType GetType() { return projType; }
	void SetProjectileType(ProjectileType type) { this->projType = type; }

	void MoveProjectile(float dt);

	bool ProjectileCollision();

private:

	ProjectileType projType;
	float projSpeed = 1.5f;

	Enemy* target = nullptr;

};

#endif // !PROJECTILE_H