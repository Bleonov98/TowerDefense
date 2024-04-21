#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

enum ProjectileType {
	ARROW_P,
	FIREBALL_P,
	ICEBALL_P
};

class Projectile : public GameObject
{
public:

	Projectile(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {

	};

	ProjectileType GetType() { return projType; }
	void SetProjectileType(ProjectileType type) { this->projType = type; }

	void MoveProjectile(float dt);

	bool ProjectileCollision(GameObject* obj);

private:

	ProjectileType projType;
	float projSpeed = 100.0f;

};

#endif // !PROJECTILE_H