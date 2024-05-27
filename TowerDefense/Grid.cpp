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

	if (cellData == 0) cellColour = glm::vec3(0.0f, 0.5f, 0.0f);
	else cellColour = glm::vec3(1.0f, 0.0f, 0.0f);

	// grid shapes
	float vertices[] = {
		-x, 0, -z,
		 x, 0, -z,
		 x, 0,  z,
		 x, 0,  z,
		-x, 0,  z,
		-x, 0, -z
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Grid::RefreshMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, cellPos);
	
	cellMatrix = modelMatrix;
}

bool Grid::CenterCollision(const glm::vec3& rayOrigin)
{
	glm::vec3 invRayDirection = 1.0f / glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 ray = glm::vec3(rayOrigin.x, cellPos.y, rayOrigin.z);

	glm::vec3 minPoint = cellPos - glm::vec3(0.02f, 0.0f, 0.02f);
	glm::vec3 maxPoint = cellPos + glm::vec3(0.02f, 0.0f, 0.02f);

	glm::vec3 tMin = (minPoint - ray) * invRayDirection;
	glm::vec3 tMax = (maxPoint - ray) * invRayDirection;

	glm::vec3 tEnter = glm::min(tMin, tMax);
	glm::vec3 tExit = glm::max(tMin, tMax);

	float tNear = glm::compMax(tEnter);
	float tFar = glm::compMin(tExit);

	return tNear <= tFar && tFar >= 0.0f && tNear >= 0.0f;
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
	glBindVertexArray(0);
}
