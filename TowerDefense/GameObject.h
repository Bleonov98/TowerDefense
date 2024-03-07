#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ResourceManager.h"

class GameObject
{
public:

	GameObject(glm::vec3 position, glm::vec3 size, float angle = 0.0f) {
		this->position = position;
		this->size = size;
		this->angle = angle;
	};

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return size; }
	glm::vec3 GetColor() { return color; }
	float GetAngle() { return angle; }

	std::string GetID() { return this->ID; }

	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetSize(glm::vec3 size) { this->size = size; }
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetAngle(float angle) { this->angle = angle; }

	void SetModel(Model model);

	void DrawObject();

	// - - - - - -

	bool IsAnimated() { return this->model.IsAnimated(); }

	bool IsDeleted() { return this->deleted; }
	void DeleteObject() { this->deleted = true; }

	virtual ~GameObject() {};

protected:

	Model model;

	std::string ID;

	bool deleted = false;
	float angle;
	glm::vec3 position, size, color = glm::vec3(1.0f);

};
#endif // !GAMEOBJECT_H