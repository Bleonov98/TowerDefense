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

	GameObject(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), glm::vec3 angle = glm::vec3(0.0f)) {
		this->position = position;
		this->scale = scale;
		this->angle = angle;
	};

	glm::mat4 GetMatrix() { return objMatrix; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return size; }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetColor() { return color; }
	glm::vec3 GetAngle() { return angle; }
	float GetTransparency() { return transparency; }
	Sphere GetHBox();

	std::string GetModelName() { return modelName; }
	std::string GetIcon() { return iconTexture; }

	// matrices, vectors
	void RefreshMatrix();
	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetScale(glm::vec3 scale) { 
		this->scale = scale;
		this->size = ResourceManager::GetModel(modelName).GetSize() * scale;
	}
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetAngle(glm::vec3 angle) { this->angle = angle; }
	void SetTransparency(float transparency) { this->transparency = transparency; }

	void SetModel(std::string modelName);
	void UpdateAnimation(Shader& shader, float dt);

	void SetIcon(std::string iconTexture) { this->iconTexture = iconTexture; }

	bool RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);
	bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2, float& outIntersectionDistance);
	virtual bool SphereCollision(GameObject* obj);

	// - - - - - -

	bool IsAnimated() { return animated; }

	bool IsDeleted() { return this->deleted; }
	void DeleteObject() { this->deleted = true; }

	virtual ~GameObject() {};

protected:

	Animator animator;

	Sphere hbox;

	std::string iconTexture = " ";
	std::string modelName;

	bool deleted = false, animated = false;
	float transparency = 0.0f;
	glm::vec3 position, scale, size, angle, color = glm::vec3(1.0f);
	glm::mat4 objMatrix;

};
#endif // !GAMEOBJECT_H
