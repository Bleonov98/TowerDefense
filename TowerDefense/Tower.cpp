#include "Tower.h"

void Tower::UpgradeTower()
{
	towerLvl++;

	damage += 30;
	attackSpeed += 25;
}

void FireTower::UpgradeTower()
{
	towerLvl++;

	damage += 5;
	attackSpeed += 50;
}

void IceTower::UpgradeTower()
{
	towerLvl++;

	damage += 15;
	attackSpeed += 10;
	slowRate += 0.3;
}
