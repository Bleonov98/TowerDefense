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

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return model.GetSize(); }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetColor() { return color; }
	float GetAngle() { return angle; }

	std::string GetID() { return model.GetName(); }

	// model should be transformed here
	void SetPosition(glm::vec3 pos);
	void SetScale(glm::vec3 scale);
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetAngle(float angle);

	void SetModel(Model model) { this->model = model; }

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
	glm::vec3 position, scale, color = glm::vec3(1.0f);

};
#endif // !GAMEOBJECT_H
