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

	if (cellData == 0) cellColour = glm::vec3(0.0f, 0.8f, 0.0f);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cellMatrix = glm::mat4(1.0f);
	cellMatrix = glm::translate(cellMatrix, cellPosition);
}

void Grid::InitInstanceBuffer(std::vector<glm::mat4> cellMatrices, std::vector<glm::vec3> cellColours)
{
	glBindVertexArray(VAO);

	// colour instance buffer
	glGenBuffers(1, &colourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cellColours.size(), &cellColours.data()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(1, 1);

	// matrix instance buffer
	glGenBuffers(1, &matVBO);
	glBindBuffer(GL_ARRAY_BUFFER, matVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * cellMatrices.size(), &cellMatrices[0], GL_STATIC_DRAW);

	// instance matrix
	std::size_t vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
}

bool Grid::CenterCollision(const glm::vec3& rayOrigin)
{
	glm::vec3 invRayDirection = 1.0f / glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 ray = glm::vec3(rayOrigin.x, cellPos.y, rayOrigin.z);

	glm::vec3 minPoint = cellPos - glm::vec3(0.05f, 0.0f, 0.05f);
	glm::vec3 maxPoint = cellPos + glm::vec3(0.05f, 0.0f, 0.05f);

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

void Grid::DrawGrid(std::vector<glm::mat4>& cellMatrices, std::vector<glm::vec3> cellColours)
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * cellColours.size(), &cellColours.data()[0]);

	glBindBuffer(GL_ARRAY_BUFFER, matVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * cellMatrices.size(), &cellMatrices.data()[0]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cellMatrices.size());
	glBindVertexArray(0);
}
