#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ResourceManager.h"

class GameObject
{
public:

	GameObject(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f) {
		this->position = position;
		this->scale = scale;
		this->angle = angle;
	};

	glm::mat4 GetMatrix() { return objMatrix; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return model.GetSize(); }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetColor() { return color; }
	float GetAngle() { return angle; }

	std::string GetID() { return model.GetName(); }
	std::string GetIcon() { return iconTexture; }

	// model should be transformed here
	void RefreshMatrix();
	void SetPosition(glm::vec3 pos);
	void SetScale(glm::vec3 scale);
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetAngle(float angle);

	void SetModel(Model model) { this->model = model; }
	void SetIcon(std::string iconTexture) { this->iconTexture = iconTexture; }

	bool RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);
	bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t);

	void DrawObject();

	// - - - - - -

	bool IsAnimated() { return this->model.IsAnimated(); }

	bool IsDeleted() { return this->deleted; }
	void DeleteObject() { this->deleted = true; }

	virtual ~GameObject() {};

protected:

	Model model;

	std::string ID, iconTexture;

	bool deleted = false;
	float angle;
	glm::vec3 position, scale, color = glm::vec3(1.0f);
	glm::mat4 objMatrix;

};
#endif // !GAMEOBJECT_H
