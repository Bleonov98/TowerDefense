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

	std::vector<glm::mat4> cellMat;
	std::vector<glm::vec3> cellCol;

	int cellData = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if ((i == 9 || i == 11 || i == 13) && 
				(j == 12 || j == 14 || j == 16)) cellData = 0;
			else cellData = 99;

			Grid* cell = new Grid(glm::vec3(startGridPos.x + j * cellWidth, startGridPos.y + gameMap->GetSize().y, startGridPos.z + i * cellHeight) + centerVec, cellWidth, cellHeight, cellData);
			grid[i][j] = cell;
			cellMat.push_back(cell->GetMatrix());
			cellCol.push_back(cell->GetColour());
		}
	}

	grid[0][0]->InitInstanceBuffer(cellMat, cellCol);
}

void Game::InitGameObjects()
{
	gameMap = new GameObject(glm::vec3(0.0f), glm::vec3(1.0f));
	gameMap->SetModel("mapModel");

	InitGrid();
	InitButtons();
}

void Game::InitButtons()
{
	Button* button;

	for (int i = 0; i < 3; i++)
	{
		button = new Button(glm::vec2(width * 0.35f + 150.0f * i, this->height - this->height * 0.17f), glm::vec2(90.0f), static_cast<ButtonID>(i), this->width, this->height);

		if (i == 0) button->AddTexture(ResourceManager::GetTexture("bowIcon"));
		else if (i == 1) button->AddTexture(ResourceManager::GetTexture("fireIcon"));
		else if (i == 2) button->AddTexture(ResourceManager::GetTexture("iceIcon"));

		buttonList.push_back(button);
	}

	button = new Button(glm::vec2(width - width * 0.09f, this->height - this->height * 0.17f), glm::vec2(90.0f), static_cast<ButtonID>(3), this->width, this->height);
	button->AddTexture(ResourceManager::GetTexture("towerIcon"));
	buttonList.push_back(button);

	button = new Button(glm::vec2(width * 0.35f, this->height - this->height * 0.17f), glm::vec2(90.0f), static_cast<ButtonID>(4), this->width, this->height);
	button->AddTexture(ResourceManager::GetTexture("powerUpIcon"));
	buttonList.push_back(button);
}

void Game::LoadResources()
{
	ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "modelShader");
	ResourceManager::LoadShader("../shaders/testVShader.vx", "../shaders/testFShader.ft", "testShader");
	ResourceManager::LoadShader("../shaders/vHudShader.vx", "../shaders/fHudShader.ft", "hudShader");
	ResourceManager::LoadShader("../shaders/indVShader.vx", "../shaders/indFShader.ft", "indShader");

	// objects

	// - - - // Models
	ResourceManager::LoadModel("map.obj", "mapModel");
	ResourceManager::LoadModel("enemy/enemy.fbx", "enemyModel");

	ResourceManager::LoadModel("arrow.obj", "arrowProjectileModel");
	// ResourceManager::LoadModel("fire.obj", "fireProjectileModel");
	// ResourceManager::LoadModel("ice.obj", "iceProjectileModel");

	ResourceManager::LoadModel("tower/tower.fbx", "arrowTowerModel");
	// ResourceManager::LoadModel("firetower/firetower.fbx", "fireTowerModel");
	ResourceManager::LoadModel("icetower/icetower.fbx", "iceTowerModel");

	// - - - // Textures
	ResourceManager::LoadTexture("HudFrame.png", true, "HUDTexture");

	ResourceManager::LoadTexture("icons/fire.png", true, "fireIcon");
	ResourceManager::LoadTexture("icons/ice.png", true, "iceIcon");
	ResourceManager::LoadTexture("icons/bow.png", true, "bowIcon");
	ResourceManager::LoadTexture("tower.png", true, "towerIcon");
	ResourceManager::LoadTexture("stats/powerUpIcon.png", true, "powerUpIcon");
	// Stat Icons
	ResourceManager::LoadTexture("stats/speed.png", true, "speedStat");
	ResourceManager::LoadTexture("stats/attack.png", true, "attackStat");
	
	// other
	ResourceManager::LoadTexture("hpStripe.png", true, "indicator");
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

			ProcessButtons();

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
	else if (gameState == END_LOSS || gameState == END_WIN && this->Keys[GLFW_KEY_ENTER]) close = true;
} 

void Game::Update(float dt)
{
	if (dt > 0.015f) dt = 0.015f;
	cout << dt << endl;

	if (gameState == ACTIVE) {

		if (!lvlStarted && timer.SecondsFromLast() >= 5) StartLevel();

		for (auto enemy : enemyList)
		{
			enemy->CheckPoint();
			enemy->Move(dt);
			if (enemy->Slipped()) player.hp -= 1;
		}

		for (auto tower : towerList)
		{
			tower->FindTarget(enemyList);

			// if target's been found
			AddProjectile(tower->Attack(dt));
		}

		for (auto projectile : projectileList)	
		{
			projectile->MoveProjectile(dt);
		}

		// light movement
		ResourceManager::GetShader("modelShader").Use();
		ResourceManager::GetShader("modelShader").SetVector3f("lightPos", light.lPos);
		ResourceManager::GetShader("modelShader").SetVector3f("lightColour", light.lColour);
		light.MoveLight(dt);

		CheckCollisions();

		if (player.hp <= 0) gameState = END_LOSS;
		else if (enemyList.empty() && player.wave == 7) gameState = END_WIN;

		if (enemyList.empty() && timer.SecondsFromLast() > 6) {
			lvlStarted = false;

			Enemy* enemy = new Enemy(glm::vec3(0.0f));
			enemy->UpgradeEnemy();
			delete enemy;

			timer.ResetLastPoint();
		}

		DeleteObjects();
	}
}

void Game::CheckCollisions()
{
	for (auto proj : projectileList)
	{
		if (proj->ProjectileCollision()) {

			int reward = proj->GetTarget()->GetGold();
			if (proj->GetTarget()->IsDeleted()) player.gold += reward; // If a target is killed, we obtain gold

			if (proj->GetType() == FIREBALL_P) { // A fireball creates flame around the target after hitting it and burns nearest targets
				ElementalEffect flame(proj->GetTarget()->GetHBox().center, proj->GetDamage());
				for (auto enemy : enemyList)
				{
					if (flame.SphereCollision(enemy) && enemy != proj->GetTarget()) {
						enemy->Hit(flame.GetDamage(), 0.0f);
						if (enemy->IsDeleted()) player.gold += reward;
					}
				}
			}
			else if (proj->GetType() == ICEBALL_P) { // A iceball creates ice breath around the target after hitting it and slows nearest targets
				Cold cold(proj->GetTarget()->GetHBox().center, proj->GetSlowRate());
				for (auto enemy : enemyList)
				{
					if (cold.SphereCollision(enemy) && enemy != proj->GetTarget()) enemy->Hit(cold.GetDamage(), cold.GetElSlowRate());
				}
			}
		}
	}
}

void Game::ProcessButtons()
{
	for (int i = 0; i < buttonList.size(); i++)
	{
		if (buttonList[i]->ButtonCollision(glm::vec2(xMouse, yMouse))) {
			if (GetActiveCell() != nullptr) {
				if (buttonList[i]->GetID() == ARROWTOWER_BUTTON) {
					Tower* tower = new Tower(GetActiveCell()->GetPosition());
					SetTower(GetActiveCell(), tower);
				}
				else if (buttonList[i]->GetID() == FIRETOWER_BUTTON) {
					FireTower* tower = new FireTower(GetActiveCell()->GetPosition());
					SetTower(GetActiveCell(), tower);
				}
				else if (buttonList[i]->GetID() == ICETOWER_BUTTON) {
					IceTower* tower = new IceTower(GetActiveCell()->GetPosition());
					SetTower(GetActiveCell(), tower);
				}
			}
			else {
				if (buttonList[i]->GetID() == SET_BUTTON) gridToggle = true; // shows grid
				else if (buttonList[i]->GetID() == UPGRADE_BUTTON) { // Upgrade tower when tower is chosen 
					auto result = std::find_if(towerList.begin(), towerList.end(), [](Tower* tower) {
						return tower->IsSelected();
						});

					if (result == towerList.end()) continue;

					if (player.gold >= (*result)->GetTowerCost() && (*result)->GetTowerLvl() <= 2) {
						player.gold -= (*result)->GetTowerCost();
						(*result)->UpgradeTower();
					}
				}
			}
		}
	}
}
	// - placement, gameplay
template<typename TowerType>
void Game::SetTower(Grid* cell, TowerType* tower)
{
	if (player.gold < tower->GetTowerCost()) return;

	if (tower->GetType() == ARROW) {
		tower->SetModel("arrowTowerModel");
		tower->SetScale(glm::vec3(0.005f));

		arrowTowerList.push_back(dynamic_cast<Tower*>(tower));
	}
	else if (tower->GetType() == FIRE) {
		tower->SetModel("fireTowerModel");

		fireTowerList.push_back(dynamic_cast<FireTower*>(tower));
	}
	else if (tower->GetType() == ICE) {
		tower->SetModel("iceTowerModel");
		tower->SetScale(glm::vec3(0.01f));

		iceTowerList.push_back(dynamic_cast<IceTower*>(tower));
	}

	player.gold -= tower->GetTowerCost();
	cell->SetCellData(static_cast<int>(tower->GetType()));
	cell->SelectCell(false);
	gridToggle = false;

	tower->SetAngle(glm::vec3(-90.0f, 0.0f, 45.0f));

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

void Game::SpawnEnemy(Indicator indicator)
{
	Enemy* enemy = new Enemy(grid[13][0]->GetPosition() - glm::vec3(0.75f * enemyList.size(), -0.1f, 0.0f));
	enemy->InitPath(grid);
	enemy->SetModel("enemyModel");
	enemy->SetIndicator(indicator);
	enemy->SetScale(glm::vec3(0.015f));
	enemy->SetAngle(glm::vec3(-90.0f, 0.0f, 0.0f));

	objList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::SpawnBoss(Indicator indicator)
{
	Boss* boss = new Boss(grid[13][0]->GetPosition(), ResourceManager::GetModel("enemyModel"));
	boss->InitPath(grid);
	boss->SetIndicator(indicator);
	boss->SetScale(glm::vec3(0.075f));

	objList.push_back(boss);
	enemyList.push_back(boss);
}

void Game::StartLevel()
{
	Indicator indicator(glm::vec2(0.5f, 0.05f));
	player.wave++;
	lvlStarted = true;

	if (player.wave < 7) {
		for (size_t i = 0; i < 500; i++)
		{
			SpawnEnemy(indicator);
		}
	}
	else SpawnBoss(indicator);
}

void Game::AddProjectile(Projectile* projectile)
{
	if (!projectile) return;

	projectileList.push_back(projectile);
	objList.push_back(projectile);

	if (projectile->GetModelName() == "arrowProjectileModel") arrowProjectileList.push_back(projectile);
	else if (projectile->GetModelName() == "fireProjectileModel") fireProjectileList.push_back(projectile);
	else if (projectile->GetModelName() == "iceProjectileModel") iceProjectileList.push_back(projectile);
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

void LightObject::MoveLight(float dt)
{
	angle -= lSpeed * dt;
	if (angle > 2.0f * glm::pi<float>()) {
		angle -= 2.0f * glm::pi<float>();
	}
	lPos = glm::vec3(lRadius * cos(angle), lRadius * sin(angle), 0.0f);
}

// Render
void Game::Render(float dt)
{
	view = camera.GetViewMatrix();

	// Objects
	DrawObject(gameMap, dt);
	
	// Towers
	glDepthMask(GL_FALSE);
	DrawObject(arrowTowerList, dt);
	DrawObject(fireTowerList, dt);
	DrawObject(iceTowerList, dt);
	glDepthMask(GL_TRUE);

	// Enemies
	DrawObject(enemyList, dt);
	DrawHP();

	// Projectiles
	DrawObject(arrowProjectileList, dt);
	DrawObject(iceProjectileList, dt);
	DrawObject(fireProjectileList, dt);

	// Interface
	HUDisplay->DrawHUD(gameState == MENU);
	DrawTowerMenu();
	DrawTowerStats();

	// Menu, Helpers
	if (gridToggle) DrawGrid();
	else buttonList[buttonList.size() - 2]->DrawButton(gameState == MENU);

	if (gameState == MENU) DrawMenuTxt();
	DrawStats();
}

template<typename T>
void Game::DrawObject(vector<T*> objectList, float dt)
{
	if (objectList.empty()) return;

	Shader& shader = ResourceManager::GetShader("modelShader").Use();
	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("view", view);

	std::string modelName = objectList[0]->GetModelName();
	std::vector<glm::mat4> objectMat;
	
	float transparency = 0.0f;
	if (gridToggle) transparency = 0.8f;
	shader.SetFloat("transparency", transparency);

	for (size_t i = 0; i < objectList.size(); i++)
	{
		// obj->UpdateAnimation(dt);
		objectList[i]->RefreshMatrix();
		objectMat.push_back(objectList[i]->GetMatrix());
	}

	ResourceManager::GetModel(modelName).DrawInstanced(shader, objectMat);
}

void Game::DrawObject(GameObject* obj, float dt)
{
	Shader& shader = ResourceManager::GetShader("modelShader").Use();
	shader.SetMatrix4("projection", projection);
	shader.SetMatrix4("view", view);

	shader.SetFloat("transparency", obj->GetTransparency());

	// obj->UpdateAnimation(dt);
	obj->RefreshMatrix();

	shader.SetMatrix4("model", obj->GetMatrix());

	ResourceManager::GetModel(obj->GetModelName()).Draw(shader);
}

void Game::DrawHP()
{
	if (enemyList.empty()) return;

	std::vector<glm::mat4> indMatVec, hpMatVec;
	std::vector<glm::vec3> colourVec;

	vector<Indicator*> indVec;

	for (auto enemy : enemyList)
	{
		enemy->SetHP();
		indMatVec.push_back(enemy->GetIndicator().GetIndicatorMatrix());
		if (enemy->FullHP()) {
			hpMatVec.push_back(enemy->GetIndicator().GetHPMatrix());
			colourVec.push_back(enemy->GetIndicator().GetColour());
		}
		else indVec.push_back(&enemy->GetIndicator());	
	}

	auto instancedEnemy = find_if(enemyList.begin(), enemyList.end(), [](Enemy* enemy) {
		return enemy->FullHP();
	});

	if (instancedEnemy != enemyList.end()) {
		(*instancedEnemy)->GetIndicator().DrawIndicator(indMatVec, hpMatVec, colourVec, projection, view, gameState == MENU);
	}

	for (size_t i = 0; i < indVec.size(); i++)
	{
		indVec[i]->DrawIndicatorsHP(projection, view, gameState == MENU);
	}
}

void Game::DrawGrid()
{
	ResourceManager::GetShader("testShader").Use();
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetMatrix4("view", view);

	std::vector<glm::mat4> cellMatrices;
	std::vector<glm::vec3> cellColours;

	for (auto rows : grid)
	{
		for (auto cell : rows)
		{
			if (cell->GetCellData() == 0) {
				cellMatrices.push_back(cell->GetMatrix());
				cellColours.push_back(cell->GetColour());
			}
		}
	}

	grid[0][0]->DrawGrid(cellMatrices, cellColours);
}

void Game::DrawStats()
{
	text->RenderText(std::to_string(player.gold), glm::vec2(1440.0f, 42.5f), 1.1f);
	text->RenderText(std::to_string(player.hp), glm::vec2(1230.0f, 40.0f), 1.1f);
	text->RenderText(std::to_string(player.wave), glm::vec2(147.0f, 38.0f), 1.5f);
}

void Game::DrawTowerMenu()
{
	string cost;
	if (GetActiveCell() != nullptr) {
		for (size_t i = 0; i < buttonList.size() - 1; i++)
		{
			if (buttonList[i]->GetID() == SET_BUTTON && gridToggle) continue;

			buttonList[i]->DrawButton(gameState == MENU);
			buttonList[i]->GetID() == ARROWTOWER_BUTTON ? cost = "50" : (buttonList[i]->GetID() == FIRETOWER_BUTTON ? cost = "75" : cost = "60");

			text->RenderText(cost, buttonList[i]->GetButtonPosition() + glm::vec2(-15.0f, 0.0f));
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
	statHUD->DrawHUD(glm::vec2(0.08f * width, 0.75f * height), glm::vec2(0.085f * height), gameState == MENU);

	// attack
	statHUD->AddTexture(ResourceManager::GetTexture("attackStat"));
	statHUD->DrawHUD(glm::vec2(0.031f * width, 0.87f * height), glm::vec2(0.034f * height), gameState == MENU);
	text->RenderText(std::to_string( (*result)->GetDamage() ), glm::vec2(100.0f, 785.0f));

	// attackspeed
	statHUD->AddTexture(ResourceManager::GetTexture("speedStat"));
	statHUD->DrawHUD(glm::vec2(0.031f * width, 0.92f * height), glm::vec2(0.034f * height), gameState == MENU);
	text->RenderText(std::to_string( (*result)->GetAttackSpeed() ), glm::vec2(100.0f, 835.0f));

	// powerUp
	if ((*result)->GetTowerLvl() <= 2) {
		buttonList.back()->DrawButton(gameState == MENU);
		text->RenderText(std::to_string((*result)->GetTowerCost()), buttonList.back()->GetButtonPosition() + glm::vec2(0.0f, 10.0f));
	}
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
		if (tower->RayCollision(rayOrigin, rayDirection) && !gridToggle) {
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

	gridToggle = false;
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

void Game::SetScreenSize(unsigned int width, unsigned int height)
{
	glm::vec2 sizeScale(static_cast<float>(width) / this->width, static_cast<float>(height) / this->height);

	this->width = width;
	this->height = height;

	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	HUDisplay->SetNewScreenSize(width, height);
	statHUD->SetNewScreenSize(width, height);

	for (size_t i = 0; i < buttonList.size(); i++)
	{
		buttonList[i]->SetNewScreenSize(width, height);
		buttonList[i]->SetButtonSize(buttonList[i]->GetButtonSize() * sizeScale);
		if (i < 3) buttonList[i]->SetButtonPosition(glm::vec2(width * 0.35f + 150.0f * sizeScale.x * i, this->height - this->height * 0.17f));
		else if (i == 3) buttonList[i]->SetButtonPosition(glm::vec2(width - width * 0.09f, this->height - this->height * 0.17f));
		else if (i == 4) buttonList[i]->SetButtonPosition(glm::vec2(width * 0.35f, this->height - this->height * 0.17f));
	}
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
	DeleteObjectFromVector(arrowTowerList, false);
	DeleteObjectFromVector(fireTowerList, false);
	DeleteObjectFromVector(iceTowerList, false);


	DeleteObjectFromVector(enemyList, false);

	DeleteObjectFromVector(projectileList, false);
	DeleteObjectFromVector(arrowProjectileList, false);
	DeleteObjectFromVector(fireProjectileList, false);
	DeleteObjectFromVector(iceProjectileList, false);

	DeleteObjectFromVector(objList, true);
}

template <typename T>
void Game::DeleteObjectFromVector(std::vector<T*>& vector, bool deleteMemory)
{
	for (auto i = vector.begin(); i != vector.end();)
	{
		if ((*i)->IsDeleted()) {
			if (deleteMemory) {

				// set null target
				Enemy* enemy = dynamic_cast<Enemy*>(*i);
				if (enemy) {
					for (auto tower : towerList)
					{
						if (tower->GetTarget() == enemy) tower->ResetTarget();
					}

					for (auto projectile : projectileList)
					{
						if (projectile->GetTarget() == enemy) projectile->ResetTarget();
					}
				}

				delete* i;
			}
			i = vector.erase(i);
		}
		else ++i;
	}
}

Game::~Game()
{
	delete text;
	delete sound;
	delete music;
	delete HUDisplay;
	delete statHUD;
	delete gameMap;

	for (auto& i : objList)
	{
		delete i;
		i = nullptr;
	}
	objList.clear();
	enemyList.clear();
	towerList.clear();
	projectileList.clear();

	for (auto& row : grid) {
		for (auto& cell : row) {
			delete cell;
			cell = nullptr;
		}
	}
	grid.clear();

	for (auto& button : buttonList)
	{
		delete button;
		button = nullptr;
	}
	buttonList.clear();
}
