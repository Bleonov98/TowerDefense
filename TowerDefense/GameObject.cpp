#include "GameObject.h"

void GameObject::SetModel(Model model)
{
	this->model = model;
	this->ID = model.GetName();
}

void GameObject::DrawObject()
{
	Shader shader = ResourceManager::GetShader("modelShader");
	model.Draw(shader);
}
