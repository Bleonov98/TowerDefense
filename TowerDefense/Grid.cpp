#include "Grid.h"

Grid::Grid(glm::vec3 cellPosition, float cellWidth, float cellHeight, int cellData)
{
	this->cellPos = cellPosition;
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->cellData = cellData;

	float x, y;
	x = cellWidth / 2.0f - 0.01f;
	y = cellHeight / 2.0f - 0.01f;

	// grid shapes
	float vertices[] = {
		-x, -y, 0,  1.0f, 1.0f, 0.0f,
		 x, -y, 0,  1.0f, 1.0f, 0.0f,
		 x,  y, 0,  1.0f, 1.0f, 0.0f,
		 x,  y, 0,  1.0f, 1.0f, 0.0f,
		-x,  y, 0,  1.0f, 1.0f, 0.0f,
		-x, -y, 0,  1.0f, 1.0f, 0.0f,
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
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	cellMatrix = modelMatrix;
}

void Grid::DrawCell()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
