#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Camera.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "HUD.h"

#include "GameObject.h"
#include "Tower.h"

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
	void LoadResources();

	// Main, GamePlay
	void ProcessInput(float dt);
	void Update(float dt);
	void CheckCollisions(float dt);

	void SetTower(glm::vec3 position);

	// Render
	void Render(float dt);
	void DrawObject(GameObject* obj, float dt);
	void DrawGrid();
	void DrawStats();
	void DrawMenuTxt();

	// Calculations
	glm::vec3 FindNearestCell(glm::vec3 position);
	glm::vec3 ClickPosition();
	glm::vec3 MouseRay();

	// Utility
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
	float cellWidth, cellHeight;
	int rows = 30, cols = 30;
	bool showGrid = false;
	std::vector<std::vector<int>> mData;
	std::vector<std::vector<glm::vec3>> grid;

	// camera, tools
	glm::vec2 cursorPos;

	glm::mat4 projection;
	glm::mat4 view;

	// objects
	std::vector<GameObject*> objList;
	std::vector<Tower*> towerList;

};

#endif // !GAME_H