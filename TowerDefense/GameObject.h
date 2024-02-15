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
	float GetAngle() { return angle; }

	void SetPosition(glm::vec3 pos) { this->position = pos; }
	void SetSize(glm::vec3 size) { this->size = size; }
	void SetAngle(float angle) { this->angle = angle; }

	void SetModel(Model model) { this->model = model; }

	void DrawObject();

	// - - - - - -

	bool IsDeleted() { return this->deleted; }
	void DeleteObject() { this->deleted = true; }

	virtual ~GameObject() {};

protected:

	Model model;

	bool deleted = false;
	float angle;
	glm::vec3 position, size;

};
#endif // !GAMEOBJECT_H