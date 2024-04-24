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
	grid.resize(rows, vector<Grid*>(cols, nullptr));

	float cellWidth = gameMap->GetSize().x / 30.0f;
	float cellHeight = gameMap->GetSize().z / 30.0f;

	glm::vec3 startGridPos = gameMap->GetPosition() - glm::vec3(gameMap->GetSize() / 2.0f);

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			Grid* cell = new Grid(glm::vec3(startGridPos.x + j * cellWidth, startGridPos.y + gameMap->GetSize().y + 0.001f, startGridPos.z + i * cellHeight) + centerVec, cellWidth, cellHeight, rand() % 2);
			grid[i][j] = cell;
		}
	}
}

void Game::InitGameObjects()
{
	InitButtons();
}

//void Game::InitButtons()
//{
//	Button* button;
//
//	for (int i = 0; i < 3; i++)
//	{
//		button = new Button(glm::vec2(450.0f + 100.0f * i, this->height - 120.0f), glm::vec2(60.0f), static_cast<ButtonID>(i), this->width, this->height);
//
//		if (i == 0) button->AddTexture(ResourceManager::GetTexture("bowIcon"));
//		else if (i == 1) button->AddTexture(ResourceManager::GetTexture("fireIcon"));
//		else if (i == 2) button->AddTexture(ResourceManager::GetTexture("iceIcon"));
//
//		buttonList.push_back(button);
//	}
//}

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

	ResourceManager::LoadTexture("icons/fire.png", true, "fireIcon");
	ResourceManager::LoadTexture("icons/ice.png", true, "iceIcon");
	ResourceManager::LoadTexture("icons/bow.png", true, "bowIcon");
}

// Main, GamePlay
	// - basics
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

			if ((xMouse > 350.0f && xMouse < 1250.0f && yMouse < 700.0f) || (yMouse < 620.0f)) glm::vec2 clickPos = ClickPosition();
			
			for (size_t i = 0; i < buttonList.size(); i++)
			{
				if (buttonList[i]->ButtonCollision(glm::vec2(xMouse, yMouse)) && GetActiveCell() != nullptr) {
					if (buttonList[i]->GetID() == ARROWTOWER_BUTTON) SetTower(GetActiveCell()->GetPosition(), ARROW);
					if (buttonList[i]->GetID() == FIRETOWER_BUTTON) SetTower(GetActiveCell()->GetPosition(), FIRE);
					if (buttonList[i]->GetID() == ICETOWER_BUTTON) SetTower(GetActiveCell()->GetPosition(), ICE);
				}
			}
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
	// - placement
void Game::SetActiveCell(Grid* cell)
{
	UnactiveCells();

	cell->SelectCell(true);
	cell->SetColour(glm::vec3(0.7f, 1.0f, 1.0f));
}

void Game::SetTower(glm::vec3 position, TowerType type)
{
	Tower* tower = nullptr;

	if (type == ARROW) tower = new Tower(position, ResourceManager::GetModel("tower"));
	else if (type == FIRE) tower = new FireTower(position, ResourceManager::GetModel("tower"));
	else if (type == ICE) tower = new IceTower(position, ResourceManager::GetModel("tower"));

	tower->SetScale(glm::vec3(1.0f, 1.0f, 0.8f));
	objList.push_back(tower);
	towerList.push_back(tower);

	UnactiveCells();
}

Grid* Game::GetActiveCell()
{
	for (auto i : grid)
	{
		for (auto cell : i)
		{
			if (cell->IsSelected()) return cell;
		}
	}

	return nullptr;
}

void Game::UnactiveCells()
{
	for (auto i : grid)
	{
		for (auto j : i)
		{
			if (j->GetCellData() == 0) {
				j->SelectCell(false);
				j->SetColour(glm::vec3(1.0f, 1.0f, 0.0f));
			}
		}
	}
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
	if (showGrid) {
		for (auto i : grid)
		{
			for (auto j : i)
			{
				DrawGrid(j);
			}

		}
	}

	if (gameState == MENU) DrawMenuTxt();

	// Interface
	HUDisplay->DrawHUD(gameState == MENU);
	DrawTowerMenu();
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

	obj->RefreshMatrix();

	ResourceManager::GetShader("modelShader").SetMatrix4("model", obj->GetMatrix());
	obj->DrawObject();
}

void Game::DrawGrid(Grid* cell)
{
	ResourceManager::GetShader("testShader").Use();
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetMatrix4("view", view);

	cell->RefreshMatrix();

	ResourceManager::GetShader("testShader").SetMatrix4("model", cell->GetMatrix());
	ResourceManager::GetShader("testShader").SetVector3f("cellColour", cell->GetColour());

	cell->DrawCell();
}

void Game::DrawStats()
{
}

void Game::DrawTowerMenu()
{
	bool found = false;
	for (auto i : grid)
	{
		for (auto cell : i)
		{
			if (cell->IsSelected()) {
				found = true; 
				break;
			}
		}
		if (found) break;
	}

	if (found) {
		for (size_t i = 0; i < buttonList.size(); i++)
		{
			buttonList[i]->DrawButton(gameState == MENU);
		}
	}
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
	glm::vec3 nearestCell;
	float minLength = 0.0f;
	bool firstCheck = true;

	for (auto i : grid)
	{
		for (auto j : i)
		{
			glm::vec3 diffVec = abs(position - j->GetPosition());
			float diffLength = sqrt(powf(diffVec.x, 2) + powf(diffVec.y, 2) + powf(diffVec.z, 2));

			if (firstCheck) {
				minLength = diffLength;
				firstCheck = false;
				nearestCell = j->GetPosition();
			}
			else if (diffLength < minLength) {
				minLength = diffLength;
				nearestCell = j->GetPosition();
			}
		}
	}

	return nearestCell;
}

glm::vec3 Game::ClickPosition()
{
	glm::vec3 clickPos(1488.0f);
	bool found = false;

	for (auto& i : grid)
	{
		for (auto& cell : i)
		{
			std::pair<glm::vec3, glm::vec3> rayValues = MouseRay(cell->GetMatrix());

			glm::vec3 rayOrigin = rayValues.first;
			glm::vec3 rayDirection = rayValues.second;

			if (cell->RayCollision(rayOrigin, rayDirection) && cell->GetCellData() == 0) { 
				clickPos = cell->GetPosition();

				SetActiveCell(cell);

				found = true;
				break;
			}
		}
		if (found) break;
	}

	if (!found) UnactiveCells();

	return clickPos;
}

std::pair<glm::vec3, glm::vec3> Game::MouseRay(glm::mat4 modelMatrix)
{
	glm::vec3 rayDirection;

	// normalized mouse position for opengl
	float normalizedX = (2.0f * xMouse) / width - 1.0f;
	float normalizedY = 1.0f - (2.0f * yMouse) / height;
	
	glm::vec4 screenPos = glm::vec4(normalizedX, normalizedY, 1.0f, 1.0f);

	glm::mat4 invProjectionView = glm::inverse(projection * view); // world from screen space transformation matrix

	// world ray values
	glm::vec3 rayOrigin = camera.GetCameraPosition();

	// Calculate direction vector
	glm::vec4 rayClip = glm::inverse(projection) * screenPos;
	rayClip = glm::vec4(rayClip.x, rayClip.y, -1.0, 0.0); // Ray points towards the -Z direction (into the screen)
	glm::vec4 rayEye = glm::inverse(view) * rayClip;
	rayDirection = glm::normalize(glm::vec3(rayEye));

	return make_pair(rayOrigin, rayDirection);
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

	// delete grid
}