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
	Grid* GetActiveCell();
	void UnactiveCells();
	void SetActiveCell(Grid* cell);
	void SetTower(Grid* cell, TowerType type);

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
	glm::vec3 ClickPosition();
	std::pair<glm::vec3, glm::vec3> MouseRay(glm::mat4 modelMatrix);

	// Utility
	void CheckGLError(const std::string& context);
	void DeleteObjects();

	template <typename T>
	void DeleteObjectFromVector(std::vector<T*>& vector, bool deleteMemory);

	~Game();
	
	// - - - - 
	bool Keys[1024], KeysProcessed[1024], mouseKeys[8], close = false;
	double xMouse, yMouse;

private:

	// basics
	GameState gameState = MENU;
	int width, height;
	bool devView = false;

	// grid
	int rows = 30, cols = 30;
	bool showGrid = false;
	std::vector<std::vector<Grid*>> grid;

	// camera, tools
	glm::vec2 cursorPos;

	glm::mat4 projection;
	glm::mat4 view;

	// objects
	std::vector<GameObject*> objList;
	std::vector<Tower*> towerList;

	std::vector<Button*> buttonList;
};

#endif // !GAME_H