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
	projectile->SetModel("arrowProjectileModel");
	projectile->SetScale(glm::vec3(0.05f));

	// rotating arrows model considering their direction 
	glm::vec3 direction = glm::normalize(target->GetPosition() - projectile->GetPosition());
	glm::vec3 baseDir = glm::vec3(0.0f, 0.0f, -1.0f);
	float dotProd = glm::dot(baseDir, direction);
	float angle = acos(dotProd / (glm::length(direction) * glm::length(baseDir)));

	float degAngle = glm::degrees(angle);
	if (direction.x > 0.0f) degAngle = -degAngle;
	projectile->SetAngle(glm::vec3(0.0f, degAngle, 0.0f));

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
	projectile->SetModel("fireProjectileModel");
	projectile->SetScale(glm::vec3(0.25f));

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
	projectile->SetModel("iceProjectileModel");
	projectile->SetScale(glm::vec3(0.1f));

	return projectile;
}
