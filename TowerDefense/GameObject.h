#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ResourceManager.h"

struct Sphere {
	glm::vec3 center;
	float radius;
};

class GameObject
{
public:

	GameObject(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) {
		this->position = position;
		this->scale = scale;
		this->angle = angle;

		if (IsAnimated()) {
			SetAnimation(ResourceManager::GetAnimation(GetID()));
			SetAnimator(ResourceManager::GetAnimator(GetID()));
		}
	};

	glm::mat4 GetMatrix() { return objMatrix; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return model.GetSize(); }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetColor() { return color; }
	float GetAngle() { return angle; }
	float GetTransparency() { return transparency; }
	Sphere GetHBox();

	std::string GetID() { return model.GetName(); }
	std::string GetIcon() { return iconTexture; }

	// model should be transformed here
	void RefreshMatrix();
	void SetPosition(glm::vec3 pos);
	void SetScale(glm::vec3 scale);
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetAngle(float angle);
	void SetTransparency(float transparency) { this->transparency = transparency; }

	void SetAnimation(Animation anim) { this->anim = anim; }
	void SetAnimator(Animator animator) { this->animator = animator; }
	void UpdateAnimation(float dt);

	void SetModel(Model model);
	void RefreshModel();

	void SetIcon(std::string iconTexture) { this->iconTexture = iconTexture; }

	bool RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);
	bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2, float& outIntersectionDistance);
	virtual bool SphereCollision(GameObject* obj);

	void DrawObject();

	// - - - - - -

	bool IsAnimated() { return this->model.IsAnimated(); }

	bool IsDeleted() { return this->deleted; }
	void DeleteObject() { this->deleted = true; }

	virtual ~GameObject() {};

protected:

	Model model;
	Animation anim;
	Animator animator;

	Sphere hbox;

	std::string iconTexture = " ";

	bool deleted = false;
	float angle, transparency = 0.0f;
	glm::vec3 position, scale, color = glm::vec3(1.0f);
	glm::mat4 objMatrix;

};
#endif // !GAMEOBJECT_H
