#ifndef GAME_H
#define GAME_H

#include <iostream>

#include "Camera.h"
#include "GameObject.h"

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

	void Init();
	void InitGrid();
	void InitGameObjects();
	void LoadResources();

	void ProcessInput(float dt);
	void Update(float dt);
	void CheckCollisions(float dt);
	void ProcessAnimations(float dt);

	void Render(float dt);
	// void DrawObject(GameObject* obj);
	void DrawStats();

	void DeleteObjects();

	// template <typename T>
	// void DeleteObjectFromVector(std::vector<T*>& vector, bool deleteMemory);

	~Game() {};
	
	// - - - - 
	bool Keys[1024], KeysProcessed[1024], close = false;

private:

	// game data

	GameState gameState = MENU;
	int width, height;

	float cellWidth, cellHeight;
	//std::vector<std::vector<int>> mData;
	//std::vector<std::vector<glm::vec2>> grid;

	//glm::vec2 cursorPos;

	//glm::mat4 projection;
	//glm::mat4 view;

	// objects

	// std::vector<GameObject*> objList;


};

#endif // !GAME_H



