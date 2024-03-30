#include "Grid.h"

Grid::Grid(glm::vec3 cellPosition, float cellWidth, float cellHeight, int cellData)
{
	this->cellPos = cellPosition;
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->cellData = cellData;

	this->cellSize = glm::vec3(cellWidth, 0.0f, cellHeight);

	float x, z;
	x = cellWidth / 2.0f - 0.01f;
	z = cellHeight / 2.0f - 0.01f;

	// grid shapes
	float vertices[] = {
		-x, 0, -z,  1.0f, 1.0f, 0.0f,
		 x, 0, -z,  1.0f, 1.0f, 0.0f,
		 x, 0,  z,  1.0f, 1.0f, 0.0f,
		 x, 0,  z,  1.0f, 1.0f, 0.0f,
		-x, 0,  z,  1.0f, 1.0f, 0.0f,
		-x, 0, -z,  1.0f, 1.0f, 0.0f,
	};

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
}

void Grid::RefreshMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, cellPos);
	
	cellMatrix = modelMatrix;
}

bool Grid::RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
{
	glm::vec3 invRayDirection = 1.0f / rayDirection;

	glm::vec3 minPoint = cellPos - cellSize / 2.0f;
	glm::vec3 maxPoint = cellPos + cellSize / 2.0f;

	glm::vec3 tMin = (minPoint - rayOrigin) * invRayDirection;
	glm::vec3 tMax = (maxPoint - rayOrigin) * invRayDirection;

	glm::vec3 tEnter = glm::min(tMin, tMax);
	glm::vec3 tExit = glm::max(tMin, tMax);

	float tNear = glm::compMax(tEnter);
	float tFar = glm::compMin(tExit);

	return tNear <= tFar && tFar >= 0.0f && tNear >= 0.0f;
}

void Grid::DrawCell()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
