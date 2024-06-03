#include "Projectile.h"

void Projectile::MoveProjectile(float dt)
{
	if (!target) {
		DeleteObject();
		return;
	}

	glm::vec3 direction = glm::normalize(target->GetPosition() - this->position);
	this->position += this->projSpeed * dt * direction;
}

bool Projectile::ProjectileCollision()
{


	return false;
}
