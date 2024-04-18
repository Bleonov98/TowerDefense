#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject
{
public:

	Projectile(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {

	};

	void MoveProjectile(float dt);

	bool ProjectileCollision(GameObject* obj);

private:

	float projSpeed = 100.0f;

};

#endif // !PROJECTILE_H