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

	Projectile(glm::vec3 position, Enemy* target, ProjectileType pType, int damage, float slowRate = 0.0f, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : GameObject(position, scale, angle) {
		this->target = target;
		this->projType = pType;
		this->damage = damage;
		this->slowRate = slowRate;
	};

	void SetSpeed(float speed) { this->projSpeed = speed; }
	void SetProjectileType(ProjectileType type) { this->projType = type; }

	float GetSpeed() { return projSpeed; }
	int GetDamage() { return this->damage; }
	float GetSlowRate() { return this->slowRate; }

	ProjectileType GetType() { return projType; }
	Enemy* GetTarget() { return this->target; }
	void ResetTarget() { this->target = nullptr; }

	void MoveProjectile(const float dt);
	bool ProjectileCollision();

private:

	ProjectileType projType;
	float projSpeed, slowRate;
	int damage;

	Enemy* target = nullptr;
};



class ElementalEffect : public GameObject {
public:
	ElementalEffect(glm::vec3 position, int damage, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : GameObject(position, scale, angle) {
		this->damage = damage;
		this->hbox.radius = 1.5f;
	};
	int GetDamage() { return this->damage; }
	bool SphereCollision(GameObject* obj) override;
protected:
	int damage;
};



struct Cold : public ElementalEffect {
public:
	Cold(glm::vec3 position, float slowRate, int damage = 0.0f, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : ElementalEffect(position, damage, scale, angle) {
		this->hbox.radius = 0.4f;
		this->slowRate = slowRate;
	};
	float GetElSlowRate() { return this->slowRate * 0.5f; };
private:
	float slowRate = 0.0f;
};


#endif // !PROJECTILE_H