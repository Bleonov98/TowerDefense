#include "GameObject.h"

void GameObject::RefreshMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	objMatrix = modelMatrix;
}

void GameObject::SetPosition(glm::vec3 pos)
{
	model.TranslateModel(pos - this->position);
	this->position = pos;
}

void GameObject::SetScale(glm::vec3 scale)
{
	model.ScaleModel(scale);
	this->scale = scale;
}

void GameObject::SetAngle(float angle)
{
	model.RotateModel(angle - this->angle);
	this->angle = angle;
}

void GameObject::DrawObject()
{
	Shader shader = ResourceManager::GetShader("modelShader");
	model.Draw(shader);
}
