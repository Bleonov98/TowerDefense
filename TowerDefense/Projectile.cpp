#include "Projectile.h"

void Projectile::MoveProjectile(const float dt)
{
	if (!target) {
		DeleteObject();
		return;
	}
	projSpeed = target->GetSpeed() + 10.0f;

	glm::vec3 direction = glm::normalize(glm::vec3(target->GetPosition().x, target->GetPosition().y + target->GetSize().y / 2.0f, target->GetPosition().z) - this->position);
	this->position += this->projSpeed * dt * direction;
}

bool Projectile::ProjectileCollision()
{
	float doubleRadius = target->GetHBox().radius + this->GetHBox().radius;
	glm::vec3 diff = target->GetHBox().center - this->GetHBox().center;

	float distance = glm::dot(diff, diff);

	if (distance <= doubleRadius) {
		DeleteObject();
		target->Hit(damage, slowRate);
		return true;
	}

	return false;
}

bool ElementalEffect::SphereCollision(GameObject* obj)
{
	float doubleRadius = obj->GetHBox().radius + hbox.radius;
	glm::vec3 diff = obj->GetHBox().center - position;

	float distance = glm::dot(diff, diff);

	return distance <= doubleRadius;
}
