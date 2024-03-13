#ifndef TOWER_H
#define TOWER_H

#include "GameObject.h"

class Tower : public GameObject
{
public:

	Tower(glm::vec3 position, glm::vec3 size, float angle = 0.0f) : GameObject(position, size, angle) {};

	// basics
	int GetAttackSpeed() { return this->attackSpeed; }
	int GetDamage() { return this->damage; }

	void SetAttackSpeed(int aSpeed) { this->attackSpeed = aSpeed; }
	void SetDamage(int damage) { this->damage = damage; }

	// gameplay

	virtual void Attack();

	void UpgradeTower();
	void CheckRadius(glm::vec3 targetPos);

private:

	int damage = 5, attackSpeed = 15;

};

class FireTower : public Tower
{
public:

private:

	int splashDamage = 3;

};

class IceTower : public Tower
{
public:

private:

	int splashDamage = 1, slow = 50;

};

#endif // !TOWER_H