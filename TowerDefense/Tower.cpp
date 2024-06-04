#include "Tower.h"

void Tower::FindTarget(std::vector<Enemy*> enemies)
{
	if (target && IsInAttackRange(target)) {
		return;
	}

	target = nullptr;
	float closestDistanceSquared = attackRadius * attackRadius;

	for (auto enemy : enemies) {
		if (IsInAttackRange(enemy)) {
			glm::vec3 diff = enemy->GetPosition() - this->GetPosition();
			float distanceSquared = glm::dot(diff, diff);

			if (distanceSquared < closestDistanceSquared) {
				closestDistanceSquared = distanceSquared;
				target = enemy;
			}
		}
	}
}

Projectile* Tower::GetProjectile()
{
	Projectile* projectile = new Projectile(this->position, target, pType, this->damage);
	projectile->SetModel(ResourceManager::GetModel("arrow"));

	return projectile;
}

Projectile* Tower::Attack(float dt)
{
	attackDelay += attackSpeed * dt;

	if (!target || attackDelay < attackSpeed) return nullptr;

	attackDelay = 0.0f;

	Projectile* projectile = GetProjectile();
	return projectile;
}

void Tower::UpgradeTower()
{
	towerLvl++;

	damage += 30;
	attackSpeed += 25;
}

bool Tower::IsInAttackRange(Enemy* enemy)
{
	glm::vec3 diff = enemy->GetPosition() - this->GetPosition();
	float distanceSquared = glm::dot(diff, diff);
	return distanceSquared <= (attackRadius * attackRadius);
}

void FireTower::UpgradeTower()
{
	towerLvl++;

	damage += 5;
	attackSpeed += 50;
}

Projectile* FireTower::GetProjectile()
{
	Projectile* projectile = new Projectile(this->position, target, pType, this->damage);
	projectile->SetModel(ResourceManager::GetModel("fire"));

	return projectile;
}

void IceTower::UpgradeTower()
{
	towerLvl++;

	damage += 15;
	attackSpeed += 10;
	slowRate += 0.3;
}

Projectile* IceTower::GetProjectile()
{
	Projectile* projectile = new Projectile(this->position, target, pType, this->damage, this->slowRate);
	projectile->SetModel(ResourceManager::GetModel("ice"));

	return projectile;
}
