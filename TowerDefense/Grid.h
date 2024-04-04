#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/component_wise.hpp"

class Grid
{
public:

	Grid(glm::vec3 cellPosition, float cellWidth, float cellHeight, int cellData = 0);

	glm::mat4 GetMatrix() { return cellMatrix; }
	glm::vec3 GetPosition() { return cellPos; }
	int GetCellData() { return cellData; }

	void SetCellPosition(glm::vec3 position) { cellPos = position; }
	void SetCellSize(float cellWidth, float cellHeight) { this->cellWidth = cellWidth, this->cellHeight = cellHeight; }
	void SetCellData(int cellData) { this->cellData = cellData; }

	void RefreshMatrix();

	void SelectCell(bool ) { selected = true; }

	bool RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

	void DrawCell();

private:

	unsigned int VBO, VAO;

	// cell
	glm::mat4 cellMatrix;
	float cellWidth, cellHeight;
	int cellData;
	glm::vec3 cellPos, cellSize;

	bool selected = false;
};

#endif // !GRID_H



