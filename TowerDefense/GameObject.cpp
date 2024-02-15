#include "GameObject.h"

void GameObject::DrawObject()
{
	Shader shader = ResourceManager::GetShader("modelShader");
	model.Draw(shader);
}
