#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

TextRenderer* text;
HUD* HUDisplay;

ISoundEngine* sound = irrklang::createIrrKlangDevice();
ISoundSource* music;

Camera camera;

GameObject* gameMap;

// variables
#define centerVec glm::vec3(cellWidth / 2.0f, 0.0f, cellHeight / 2.0f)

// Initialization, Loading
void Game::Init()
{
	srand(time(NULL));
	LoadResources();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sounds
	// music = sound->addSoundSourceFromFile("../sounds/music.mp3");
	// music->setDefaultVolume(0.5f);

	// sound->play2D(music, true);

	// tools
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	text = new TextRenderer(this->width, this->height);
	text->Load("../fonts/Garamond.ttf", 24);

	HUDisplay = new HUD(this->width, this->height);
	HUDisplay->AddTexture(ResourceManager::GetTexture("HUDTexture"));

	cursorPos = glm::vec2(this->width / 2.0f - 50.0f, this->height / 2.0f);

	gameMap = new GameObject(glm::vec3(0.0f), glm::vec3(1.0f));
	gameMap->SetModel(ResourceManager::GetModel("map"));

	InitGrid();
	InitGameObjects();
}

void Game::InitGrid()
{
	grid.resize(rows, std::vector<glm::vec3>(cols, glm::vec3(0.0f)));
	mData.resize(rows, std::vector<int>(cols, 0));

	cellWidth = gameMap->GetSize().x / 30.0f;
	cellHeight = gameMap->GetSize().z / 30.0f;

	glm::vec3 startGridPos = gameMap->GetPosition() - glm::vec3(gameMap->GetSize() / 2.0f);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			grid[i][j] = glm::vec3(startGridPos.x + j * cellWidth, startGridPos.y + gameMap->GetSize().y + 0.001f, startGridPos.z + i * cellHeight);
		}
	}
}

void Game::InitGameObjects()
{

}

void Game::LoadResources()
{
	ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "modelShader");
	ResourceManager::LoadShader("../shaders/testVShader.vx", "../shaders/testFShader.ft", "testShader");

	// objects

	// - - - // Models
	ResourceManager::LoadModel("map.obj", "map");
	ResourceManager::LoadModel("tower.obj", "tower");

	// - - - // Textures
	ResourceManager::LoadTexture("HudFrame.png", true, "HUDTexture");
}

// Main, GamePlay
void Game::ProcessInput(float dt)
{
	if (gameState == ACTIVE) {

		if (devView) {
			if (this->Keys[GLFW_KEY_UP]) camera.ProcessKeyboard(UP, dt);
			if (this->Keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(DOWN, dt);
			if (this->Keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, dt);
			if (this->Keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, dt);
			if (this->Keys[GLFW_KEY_KP_ADD]) camera.ProcessKeyboard(FORWARD, dt);
			if (this->Keys[GLFW_KEY_KP_SUBTRACT]) camera.ProcessKeyboard(BACKWARD, dt);

			float prevPitch = camera.Pitch;

			if (this->Keys[GLFW_KEY_W] && camera.Pitch < 89.5f) camera.Pitch += 0.1f;
			else if (this->Keys[GLFW_KEY_S] && camera.Pitch > -89.5f) camera.Pitch -= 0.1f;

			camera.updateCameraVectors();
			if (prevPitch != camera.Pitch) cout << std::format("pitch: {}", static_cast<int>(camera.Pitch)) << endl;
		}

		if (this->mouseKeys[GLFW_MOUSE_BUTTON_LEFT]) {
			glm::vec3 towerPos = FindNearestCell(glm::vec3(xMouse, 0.0f, yMouse));

			SetTower(towerPos);
		}

		if (this->Keys[GLFW_KEY_G]) showGrid = true;
		else showGrid = false;

		if (this->Keys[GLFW_KEY_V] && !KeysProcessed[GLFW_KEY_V]) {
			devView = !devView;
			this->KeysProcessed[GLFW_KEY_V] = true;
		}

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

void Game::CheckCollisions(float dt)
{
}

void Game::SetTower(glm::vec3 position)
{
	Tower* tower = new Tower(position);
	tower->SetModel(ResourceManager::GetModel("tower"));
	tower->SetScale(glm::vec3(1.0f, 1.0f, 0.8f));

	objList.push_back(tower);
	towerList.push_back(tower);
}

// Render
void Game::Render(float dt)
{
	view = camera.GetViewMatrix();

	// Objects
	DrawObject(gameMap, dt);
	
	for (auto i : towerList)
	{
		DrawObject(i, dt);
	}

	// Menu, Helpers
	if (showGrid) DrawGrid();

	if (gameState == MENU) DrawMenuTxt();

	// Interface
	HUDisplay->DrawHUD(gameState == MENU);
}

void Game::DrawObject(GameObject* obj, float dt)
{
	if (obj->IsAnimated()) ResourceManager::GetAnimator(obj->GetID()).UpdateAnimation(dt);

	ResourceManager::GetShader("modelShader").Use();
	ResourceManager::GetShader("modelShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("modelShader").SetMatrix4("view", view);

	if (obj->IsAnimated()) {
		ResourceManager::GetShader("modelShader").SetBool("animated", true);

		auto transforms = ResourceManager::GetAnimator(obj->GetID()).GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ResourceManager::GetShader("modelShader").SetMatrix4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
	}
	else ResourceManager::GetShader("modelShader").SetBool("animated", false);


	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, obj->GetPosition());
	model = glm::scale(model, obj->GetScale());

	ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
	obj->DrawObject();
}

void Game::DrawGrid()
{
	float x, y;
	x = cellWidth / 2.0f - 0.01f;
	y = cellHeight / 2.0f - 0.01f;

	// grid shapes
	float vertices[] = {
		-x, -y, 0,  1.0f, 1.0f, 0.0f,
		 x, -y, 0,  1.0f, 1.0f, 0.0f,
		 x,  y, 0,  1.0f, 1.0f, 0.0f,
		 x,  y, 0,  1.0f, 1.0f, 0.0f,
		-x,  y, 0,  1.0f, 1.0f, 0.0f,
		-x, -y, 0,  1.0f, 1.0f, 0.0f,
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// vertex colours
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	ResourceManager::GetShader("testShader").Use();
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetMatrix4("view", view);

	glm::vec3 place = glm::vec3(1.0f - gameMap->GetSize().x / 2.0f, 1.0f, 1.0f - gameMap->GetSize().z / 2.0f);

	// drawing grid
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, grid[i][j] + glm::vec3(cellWidth / 2.0f, 0.0f, cellHeight / 2.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			ResourceManager::GetShader("testShader").SetMatrix4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

void Game::DrawStats()
{
}

void Game::DrawMenuTxt()
{
	text->RenderText("MENU", glm::vec2(this->width / 2.0f - 65.0f, this->height / 2.0f - 116.0f), 1.75f, glm::vec3(0.75f));

	text->RenderText("Start", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f), 1.0f, glm::vec3(1.0f));
	text->RenderText("Exit", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f + 40.0f), 1.0f, glm::vec3(1.0f));

	text->RenderText("->", glm::vec2(cursorPos), 1.0f, glm::vec3(1.0f));
}


// Calculations
glm::vec3 Game::FindNearestCell(glm::vec3 position)
{
	std::vector<float> lengthVec;

	for (auto i : grid)
	{
		for (auto j : i)
		{
			glm::vec3 diffVec = abs(position - j);
			lengthVec.push_back(sqrt(powf(diffVec.x, 2) + powf(diffVec.y, 2) + powf(diffVec.z, 2)));
		}
	}

	return glm::vec3();
}

// Utility
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

