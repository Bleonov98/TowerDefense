#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/epsilon.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include "Timer.h"
#include "Camera.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "Grid.h"
#include "HUD.h"

#include "GameObject.h"
#include "Tower.h"
#include "Projectile.h"
#include "Enemy.h"

enum GameState {
	ACTIVE,
	MENU,
	END_WIN,
	END_LOSS
};

struct Player {
	int gold = 55;
	int hp = 5;
	int wave = 0;
};

struct LightObject {
public:
	LightObject() {};
	glm::vec3 lPos = glm::vec3(lRadius * cos(angle), lRadius * sin(angle), 0.0f), lColour = glm::vec3(1.2f, 1.2f, 1.0f);
	void MoveLight(float dt);
private:
	float lRadius = 10.0f, lSpeed = 0.1f, angle = -90.0f;
};

class Game
{
public:

	Game(const int width, const int height) {
		this->width = width, this->height = height;

		cursorPos = glm::vec2(this->width / 2.0f - 50.0f, this->height / 2.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	};

	// Initialization, Loading
	void Init();
	void InitGrid();
	void InitGameObjects();
	void InitButtons();
	void LoadResources();

	// Main, GamePlay
	void ProcessInput(float dt);
	void Update(float dt);
	void CheckCollisions();
	void ProcessButtons();

	// placement
	void UnactiveCells();
	Grid* GetActiveCell();
	void SetActiveCell(Grid* cell);

	template<typename Tower>
	void SetTower(Grid* cell, Tower* tower);
	void UnselectTowers();

	void SpawnEnemy(Indicator indicator);
	void SpawnBoss(Indicator indicator);
	void StartLevel();
	void AddProjectile(Projectile* projectile);

	// Render
	void Render(float dt);
	void DrawObject(GameObject* obj, float dt);
	void DrawGrid(Grid* cell);

	void DrawTowerMenu();
	void DrawTowerStats();

	void DrawStats();
	void DrawMenuTxt();

	// Calculations
	glm::vec3 FindNearestCell(glm::vec3 position);
	glm::vec3 ProcessClick();
	std::pair<glm::vec3, glm::vec3> MouseRay();

	// Utility
	void CheckGLError(const std::string& context);
	void DeleteObjects();
	template <typename T>
	void DeleteObjectFromVector(std::vector<T*>& vector, bool deleteMemory);

	~Game();
	
	// - - - - 
	bool Keys[1024], KeysProcessed[1024], mouseKeys[8], mKeysProcessed[8], close = false;
	double xMouse, yMouse;

private:

	// basics
	GameState gameState = MENU;
	int width, height;
	// bool devView = false;

	// grid
	int rows = 30, cols = 30;
	bool gridToggle = false, lvlStarted = false;
	std::vector<std::vector<Grid*>> grid;

	// camera, tools
	glm::vec2 cursorPos;

	glm::mat4 projection;
	glm::mat4 view;

	// objects
	Timer timer;
	LightObject light;
	Player player;

	std::vector<GameObject*> objList;
	std::vector<Tower*> towerList;
	std::vector<Enemy*> enemyList;
	std::vector<Projectile*> projectileList;

	std::vector<Button*> buttonList;
};

#endif // !GAME_H