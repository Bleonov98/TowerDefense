#ifndef TOWER_H
#define TOWER_H

#include "GameObject.h"

enum TowerType {
	ARROW = 1,
	FIRE,
	ICE
};

class Tower : public GameObject
{
public:

	Tower(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : GameObject(position, scale, angle) {
		this->model = model;
		this->iconTexture = "bowIcon";

		RefreshModel();
	};

	// basics
	int GetAttackSpeed() { return this->attackSpeed; }
	int GetDamage() { return this->damage; }
	int GetTowerLvl() { return this->towerLvl; }
	int GetTowerCost() { return this->towerCost; }
	
	void SetAttackSpeed(int aSpeed) { this->attackSpeed = aSpeed; }
	void SetDamage(int damage) { this->damage = damage; }

	void SelectTower(bool select) { this->selected = select; }
	bool IsSelected() { return selected; }
	
	// gameplay
	// virtual void Attack();
	// void CheckRadius(glm::vec3 targetPos);
	
	virtual void UpgradeTower();


protected:

	int damage = 30, attackSpeed = 25, towerLvl = 1, towerCost = 50;
	bool selected = false;

};

class FireTower : public Tower
{
public:

	FireTower(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : Tower(position, model, scale, angle) {
		this->iconTexture = "fireIcon";

		this->damage = 5;
		this->attackSpeed = 50;
		this->towerCost = 100;
	};

	void UpgradeTower() override;

private:

	

};

class IceTower : public Tower
{
public:

	IceTower(glm::vec3 position, Model model, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) : Tower(position, model, scale, angle) {
		this->iconTexture = "iceIcon";

		this->damage = 15;
		this->attackSpeed = 10;
		this->towerCost = 100;
	};

	void UpgradeTower() override;

private:

	int slowRate = 0.2;

};

#endif // !TOWER_H