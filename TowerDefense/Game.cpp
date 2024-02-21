#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

TextRenderer* text;
ISoundEngine* sound = irrklang::createIrrKlangDevice();
ISoundSource* music;

Camera camera;

GameObject* map;

void Game::Init()
{
	srand(time(NULL));
	LoadResources();

	InitGrid();
	InitGameObjects();
}

void Game::InitGrid()
{
}

void Game::InitGameObjects()
{
}

void Game::LoadResources()
{
	ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "modelShader");
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render(float dt)
{
}

void Game::DrawObject(GameObject* obj)
{
	ResourceManager::GetShader("modelShader").Use();
	ResourceManager::GetShader("modelShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("modelShader").SetMatrix4("view", view);

	ResourceManager::GetShader("modelShader").SetVector3f("objectColor", obj->GetColor());
	// modelShader.SetVector3f("lightColor", light.GetColor());
	// modelShader.SetVector3f("lightPos", light.GetPosition());
	// modelShader.SetVector3f("viewPos", camera.GetCameraPosition());

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, obj->GetPosition());
	model = glm::scale(model, obj->GetSize());

	ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
	obj->DrawObject();
}

void Game::DrawMenu()
{
	text->RenderText("MENU", glm::vec2(this->width / 2.0f - 65.0f, this->height / 2.0f - 116.0f), 1.75f, glm::vec3(0.75f));

	text->RenderText("Start", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f), 1.0f, glm::vec3(1.0f));
	text->RenderText("Exit", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f + 40.0f), 1.0f, glm::vec3(1.0f));

	text->RenderText("->", glm::vec2(cursorPos), 1.0f, glm::vec3(1.0f));
}

void Game::DeleteObjects()
{

	DeleteObjectFromVector(objList, true);
}

template <typename T>
void Game::DeleteObjectFromVector(std::vector<T*>& vector, bool deleteMemory)
{
	for (auto i = vector.begin(); i != vector.end();)
	{
		if ((*i)->IsDeleted()) {
			if (deleteMemory) delete* i;
			i = vector.erase(i);
		}
		else ++i;
	}
}

Game::~Game()
{
	delete text;
	delete sound;

	for (auto i : objList)
	{
		delete i;
	}
	objList.clear();
}

