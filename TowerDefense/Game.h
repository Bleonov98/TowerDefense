#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/epsilon.hpp>

#include <iostream>

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
	END
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
	void CheckCollisions(float dt);

	// placement
	void UnactiveCells();
	Grid* GetActiveCell();
	void SetActiveCell(Grid* cell);

	void SetTower(Grid* cell, TowerType type);
	void UnselectTowers();

	void SpawnEnemy();
	
	void StartLevel();
		
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
	bool gridToggle = false;
	std::vector<std::vector<Grid*>> grid;

	// camera, tools
	glm::vec2 cursorPos;

	glm::mat4 projection;
	glm::mat4 view;

	// objects
	std::vector<GameObject*> objList;
	std::vector<Tower*> towerList;
	std::vector<Enemy*> enemyList;
	std::vector<Projectile*> projectileList;

	std::vector<Button*> buttonList;
};

#endif // !GAME_H