#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

HUD* HUDisplay;
HUD* statHUD;

TextRenderer* text;

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

	text = new TextRenderer(this->width, this->height);
	text->Load("../fonts/Garamond.ttf", 24);

	// HUD
	HUDisplay = new HUD(this->width, this->height);
	HUDisplay->AddTexture(ResourceManager::GetTexture("HUDTexture"));
	statHUD = new HUD(this->width, this->height);

	InitGameObjects();
}

void Game::InitGrid()
{
	grid.resize(rows, vector<Grid*>(cols, nullptr));

	float cellWidth = gameMap->GetSize().x / rows;
	float cellHeight = gameMap->GetSize().z / cols;

	glm::vec3 startGridPos = gameMap->GetPosition() - glm::vec3(gameMap->GetSize() / 2.0f);
	startGridPos.y = gameMap->GetPosition().y - gameMap->GetSize().y + 0.11f;

	int cellData = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if (i >= 7 && i <= 18 && j >= 11 && j <= 17) cellData = 0;
			else cellData = 99;

			Grid* cell = new Grid(glm::vec3(startGridPos.x + j * cellWidth, startGridPos.y + gameMap->GetSize().y, startGridPos.z + i * cellHeight) + centerVec, cellWidth, cellHeight, cellData);
			grid[i][j] = cell;
		}
	}
}

void Game::InitGameObjects()
{
	gameMap = new GameObject(glm::vec3(0.0f), glm::vec3(1.0f));
	gameMap->SetModel(ResourceManager::GetModel("map"));

	InitGrid();
	InitButtons();
}

void Game::InitButtons()
{
	Button* button;

	for (int i = 0; i < 3; i++)
	{
		button = new Button(glm::vec2(550.0f + 150.0f * i, this->height - 145.0f), glm::vec2(90.0f), static_cast<ButtonID>(i), this->width, this->height);

		if (i == 0) button->AddTexture(ResourceManager::GetTexture("bowIcon"));
		else if (i == 1) button->AddTexture(ResourceManager::GetTexture("fireIcon"));
		else if (i == 2) button->AddTexture(ResourceManager::GetTexture("iceIcon"));

		buttonList.push_back(button);
	}

	button = new Button(glm::vec2(width - 100.0f, height - 145.0f), glm::vec2(90.0f), static_cast<ButtonID>(3), this->width, this->height);
	button->AddTexture(ResourceManager::GetTexture("towerIcon"));
	buttonList.push_back(button);
}

void Game::LoadResources()
{
	ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "modelShader");
	ResourceManager::LoadShader("../shaders/testVShader.vx", "../shaders/testFShader.ft", "testShader");
	ResourceManager::LoadShader("../shaders/vHudShader.vx", "../shaders/fHudShader.ft", "hudShader");

	// objects

	// - - - // Models
	ResourceManager::LoadModel("map.obj", "map");
	ResourceManager::LoadModel("tower.obj", "tower");
	ResourceManager::LoadModel("enemy.obj", "enemy");
	// ResourceManager::LoadModel("boss.obj", "boss");

	// - - - // Textures
	ResourceManager::LoadTexture("HudFrame.png", true, "HUDTexture");

	ResourceManager::LoadTexture("icons/fire.png", true, "fireIcon");
	ResourceManager::LoadTexture("icons/ice.png", true, "iceIcon");
	ResourceManager::LoadTexture("icons/bow.png", true, "bowIcon");

	ResourceManager::LoadTexture("stats/speed.png", true, "speedStat");
	ResourceManager::LoadTexture("stats/attack.png", true, "attackStat");

	ResourceManager::LoadTexture("tower.png", true, "towerIcon");
}

// Main, GamePlay
	// - basics
void Game::ProcessInput(float dt)
{
	if (gameState == ACTIVE) {

		/* if (devView) {
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
		}*/


		// mouse click
		if (this->mouseKeys[GLFW_MOUSE_BUTTON_LEFT] && !mKeysProcessed[GLFW_MOUSE_BUTTON_LEFT]) {

			if ((xMouse > 350.0f && xMouse < 1250.0f && yMouse < 700.0f) || (yMouse < 620.0f)) ProcessClick();
			
			for (size_t i = 0; i < buttonList.size(); i++)
			{
				if (buttonList[i]->ButtonCollision(glm::vec2(xMouse, yMouse))) {
					if (buttonList[i]->GetID() == ARROWTOWER_BUTTON) SetTower(GetActiveCell(), ARROW);
					else if (buttonList[i]->GetID() == FIRETOWER_BUTTON) SetTower(GetActiveCell(), FIRE);
					else if (buttonList[i]->GetID() == ICETOWER_BUTTON) SetTower(GetActiveCell(), ICE);
					else if (buttonList[i]->GetID() == MENU_BUTTON) gridToggle = true;
				}
			}

			mKeysProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
		}

		//if (this->Keys[GLFW_KEY_V] && !KeysProcessed[GLFW_KEY_V]) {
		//	devView = !devView;
		//	this->KeysProcessed[GLFW_KEY_V] = true;
		//}

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
	static bool lvlStarted = false;
	if (gameState == ACTIVE) {

		if (!lvlStarted) {
			StartLevel();
			lvlStarted = true;
		}

		for (auto enemy : enemyList)
		{
			enemy->CheckPoint();
			enemy->Move(dt);
		}
	}
}

void Game::StartLevel()
{
	SpawnEnemy();
}

void Game::CheckCollisions(float dt)
{
	
}
	// - placement
void Game::SetTower(Grid* cell, TowerType type)
{
	Tower* tower = nullptr;

	if (type == ARROW) tower = new Tower(cell->GetPosition(), ResourceManager::GetModel("tower"));
	else if (type == FIRE) tower = new FireTower(cell->GetPosition(), ResourceManager::GetModel("tower"));
	else if (type == ICE) tower = new IceTower(cell->GetPosition(), ResourceManager::GetModel("tower"));

	cell->SetCellData(static_cast<int>(type));
	cell->SelectCell(false);
	gridToggle = false;

	tower->SetScale(glm::vec3(0.9f, 1.0f, 0.8f));
	objList.push_back(tower);
	towerList.push_back(tower);
}

void Game::UnselectTowers()
{
	for (auto i : towerList)
	{
		if (i->IsSelected()) i->SelectTower(false);
	}
}

void Game::SpawnEnemy()
{
	Enemy* enemy = new Enemy(grid[13][0]->GetPosition(), ResourceManager::GetModel("enemy"));
	enemy->SetScale(glm::vec3(0.25f));
	enemy->InitPath(grid);
	objList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::SetActiveCell(Grid* cell)
{
	cell->SelectCell(true);
	cell->SetColour(glm::vec3(0.7f, 1.0f, 1.0f));

	cout << cell->GetPosition().x << " " << cell->GetPosition().y << " " << cell->GetPosition().z << endl;
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
				j->SetColour(glm::vec3(0.0f, 0.8f, 0.0f));
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

	for (auto enemy : enemyList)
	{
		DrawObject(enemy, dt);
	}

	// Interface
	HUDisplay->DrawHUD(gameState == MENU);
	DrawTowerMenu();
	DrawTowerStats();

	// Menu, Helpers
	if (gridToggle) {
		for (auto i : grid)
		{
			for (auto j : i)
			{
				DrawGrid(j);
			}
		}
	}
	else buttonList.back()->DrawButton(gameState == MENU);

	if (gameState == MENU) DrawMenuTxt();
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

	if (cell->GetCellData() == 0) cell->DrawCell();
}

void Game::DrawStats()
{
}

void Game::DrawTowerMenu()
{
	if (GetActiveCell() != nullptr) {
		for (size_t i = 0; i < buttonList.size() - 1; i++)
		{
			buttonList[i]->DrawButton(gameState == MENU);
		}
	}
}

void Game::DrawTowerStats()
{
	auto result = std::find_if(towerList.begin(), towerList.end(), [](Tower* tower) {
		return tower->IsSelected();
	});

	if (result == towerList.end()) return;

	// tower
	statHUD->AddTexture(ResourceManager::GetTexture( (*result)->GetIcon() ));
	statHUD->DrawHUD(glm::vec2(125.0f, 670.0f), glm::vec2(75.0f), gameState == MENU);

	// attack
	statHUD->AddTexture(ResourceManager::GetTexture("attackStat"));
	statHUD->DrawHUD(glm::vec2(50.0f, 780.0f), glm::vec2(30.0f), gameState == MENU);
	text->RenderText(std::to_string( (*result)->GetDamage() ), glm::vec2(100.0f, 785.0f));

	// attackspeed
	statHUD->AddTexture(ResourceManager::GetTexture("speedStat"));
	statHUD->DrawHUD(glm::vec2(50.0f, 830.0f), glm::vec2(30.0f), gameState == MENU);
	text->RenderText(std::to_string( (*result)->GetAttackSpeed() ), glm::vec2(100.0f, 835.0f));
}

void Game::DrawMenuTxt()
{
	text->RenderText("MENU", glm::vec2(this->width / 2.0f - 65.0f, this->height / 2.0f - 116.0f), 1.75f, glm::vec3(0.75f));

	text->RenderText("Start", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f));
	text->RenderText("Exit", glm::vec2(this->width / 2.0f - 20.0f, this->height / 2.0f + 40.0f));

	text->RenderText("->", glm::vec2(cursorPos));
}

// Calculations
glm::vec3 Game::FindNearestCell(glm::vec3 position)
{
	glm::vec3 nearestCell;
	float minLength = std::numeric_limits<float>::max();

	for (auto row : grid)
	{
		for (auto cell : row)
		{
			glm::vec3 diffVec = abs(position - cell->GetPosition());
			float diffLength = sqrt(powf(diffVec.x, 2) + powf(diffVec.y, 2) + powf(diffVec.z, 2));

			if (diffLength < minLength) {
				minLength = diffLength;
				nearestCell = cell->GetPosition();
			}
		}
	}

	return nearestCell;
}

glm::vec3 Game::ProcessClick() 
{
	UnselectTowers();
	UnactiveCells();

	std::pair<glm::vec3, glm::vec3> rayValues = MouseRay();

	glm::vec3 rayOrigin = rayValues.first;
	glm::vec3 rayDirection = rayValues.second;

	// check tower click
	for (auto& tower : towerList)
	{
		if (tower->RayCollision(rayOrigin, rayDirection)) {
			tower->SelectTower(true);
			
			return tower->GetPosition();
		}
	}

	// check grid click
	for (auto& i : grid)
	{
		for (auto& cell : i)
		{
			if (cell->RayCollision(rayOrigin, rayDirection) && cell->GetCellData() == 0 && gridToggle) {
				SetActiveCell(cell);

				return cell->GetPosition();
			}
		}
	}

	return glm::vec3(0.0f);
}

std::pair<glm::vec3, glm::vec3> Game::MouseRay()
{
	float normalizedX = (2.0f * xMouse) / width - 1.0f;
	float normalizedY = 1.0f - (2.0f * yMouse) / height;

	glm::vec4 rayClip = glm::vec4(normalizedX, normalizedY, -1.0f, 1.0f);
	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
	rayEye.z = -1.0f;
	rayEye.w = 0.0f;

	glm::vec4 rayWorld4 = glm::inverse(view) * rayEye;
	glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));

	glm::vec3 rayOrigin = camera.GetCameraPosition();

	return std::make_pair(rayOrigin, rayWorld);
}

// Utility
void Game::CheckGLError(const std::string& context)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error in " << context << ": " << err << std::endl;
	}
}

void Game::DeleteObjects()
{
	DeleteObjectFromVector(towerList, false);
	DeleteObjectFromVector(enemyList, false);
	DeleteObjectFromVector(projectileList, false);

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

	// delete grid, buttons, hud, clear all vectors
}