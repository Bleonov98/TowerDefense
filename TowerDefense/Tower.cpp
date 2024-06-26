#include "Tower.h"

void Tower::FindTarget(std::vector<Enemy*> enemies)
{
	if (target && IsInAttackRange(target)) return;

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
	projectile->SetModel(ResourceManager::GetModel("arrowProjectileModel"));

	return projectile;
}

Projectile* Tower::Attack(float dt)
{
	attackDelay += attackSpeed * dt;

	if (!target || attackDelay < 20.0f) return nullptr;

	attackDelay = 0.0f;

	Projectile* projectile = GetProjectile();
	return projectile;
}

void Tower::UpgradeTower()
{
	towerLvl++;

	damage += 35;
	attackSpeed += 20;
	towerCost += 25;
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

	damage += 10;
	attackSpeed += 40;
	towerCost += 35;
}

Projectile* FireTower::GetProjectile()
{
	Projectile* projectile = new Projectile(this->position, target, pType, this->damage);
	projectile->SetModel(ResourceManager::GetModel("fireProjectileModel"));

	return projectile;
}

void IceTower::UpgradeTower()
{
	towerLvl++;

	damage += 20;
	attackSpeed += 10;
	slowRate += 0.3;
	towerCost += 30;
}

Projectile* IceTower::GetProjectile()
{
	Projectile* projectile = new Projectile(this->position, target, pType, this->damage, this->slowRate);
	projectile->SetModel(ResourceManager::GetModel("iceProjectileModel"));

	return projectile;
}
