#ifndef TOWER_H
#define TOWER_H

#include "GameObject.h"
#include "Enemy.h"
#include "Projectile.h"

enum TowerType {
	ARROW = 1,
	FIRE,
	ICE
};

class Tower : public GameObject
{
public:

	Tower(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : GameObject(position, scale, angle) {
		this->iconTexture = "bowIcon";
		towerType = ARROW;
	};

	// basics
	int GetAttackSpeed() { return this->attackSpeed; }
	int GetDamage() { return this->damage; }
	int GetTowerLvl() { return this->towerLvl; }
	int GetTowerCost() { return this->towerCost; }
	
	void SetAttackSpeed(int aSpeed) { this->attackSpeed = aSpeed; }
	void SetDamage(int damage) { this->damage = damage; }

	TowerType GetType() { return towerType; }
	void SelectTower(bool select) { this->selected = select; }
	bool IsSelected() { return selected; }
	
	// gameplay
	void FindTarget(std::vector<Enemy*> enemies);
	Enemy* GetTarget() { return this->target; }
	void ResetTarget() { this->target = nullptr; }

	virtual Projectile* GetProjectile();
	Projectile* Attack(float dt);
	
	virtual void UpgradeTower();

	virtual ~Tower() {};

protected:

	int damage = 35, attackSpeed = 25, towerLvl = 1, towerCost = 50;
	float attackRadius = 5.0f, attackDelay = 0.0f;
	bool selected = false;
	Enemy* target = nullptr;
	TowerType towerType;
	ProjectileType pType = ARROW_P;

	bool IsInAttackRange(Enemy* enemy);
};

class FireTower : public Tower
{
public:

	FireTower(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : Tower(position, scale, angle) {
		this->iconTexture = "fireIcon";
		towerType = FIRE;

		this->damage = 10;
		this->attackSpeed = 50;
		this->towerCost = 75;

		pType = FIREBALL_P;
	};

	void UpgradeTower() override;
	Projectile* GetProjectile() override;

private:

	

};

class IceTower : public Tower
{
public:

	IceTower(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) : Tower(position, scale, angle) {
		this->iconTexture = "iceIcon";
		towerType = ICE;

		this->damage = 10;
		this->attackSpeed = 25;
		this->towerCost = 60;

		pType = ICEBALL_P;
	};

	void UpgradeTower() override;
	Projectile* GetProjectile() override;

private:

	float slowRate = 0.2;

};

#endif // !TOWER_H