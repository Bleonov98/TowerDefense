#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

TextRenderer* text;
ISoundEngine* sound = irrklang::createIrrKlangDevice();
ISoundSource* music;

Camera camera;

GameObject* gameMap;

void Game::Init()
{
	srand(time(NULL));
	LoadResources();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sounds
	//music = sound->addSoundSourceFromFile("../sounds/music.mp3");
	//music->setDefaultVolume(0.5f);

	//sound->play2D(music, true);

	// tools
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	text = new TextRenderer(this->width, this->height);
	text->Load("../fonts/Garamond.ttf", 24);

	cursorPos = glm::vec2(this->width / 2.0f - 50.0f, this->height / 2.0f);

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
	if (gameState == ACTIVE) {


		if (this->Keys[GLFW_KEY_ESCAPE] && !KeysProcessed[GLFW_KEY_ESCAPE]) {
			gameState = MENU;
			KeysProcessed[GLFW_KEY_ESCAPE] = true;
		}
	}
	else if (gameState == MENU) {
		if (this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP] && cursorPos.y > this->height / 2.0f) {
			cursorPos.y -= 40.0f;
			this->KeysProcessed[GLFW_KEY_UP] = true;
		}
		else if (this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN] && cursorPos.y < this->height / 2.0f + 40.0f) {
			cursorPos.y += 40.0f;
			this->KeysProcessed[GLFW_KEY_DOWN] = true;
		}
		else if (this->Keys[GLFW_KEY_ENTER]) {
			if (cursorPos.y == this->height / 2.0f) gameState = ACTIVE;
			else if (cursorPos.y == this->height / 2.0f + 40.0f) close = true;
		}
	}
	else if (gameState == END && this->Keys[GLFW_KEY_ENTER]) close = true;
}

void Game::Update(float dt)
{
	if (gameState == ACTIVE) {

	}
}

void Game::Render(float dt)
{
	// DrawObject(gameMap);

	if (gameState == MENU) DrawMenu();
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

