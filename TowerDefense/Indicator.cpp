#include "Indicator.h"

Indicator::Indicator(glm::vec3 position, glm::vec2 size)
{
	this->position = position;
	indSize = this->size = size;

	// halves
	float x, y;
	x = size.x / 2.0f;
	y = size.y / 2.0f;

	// indicator shapes
	float vertices[] = {
		-x,  y,  0, 0.0f, 1.0f,
		 x, -y,  0, 1.0f, 0.0f,
		-x, -y,  0, 0.0f, 0.0f,

		-x,  y,  0, 0.0f, 1.0f,
		 x,  y,  0, 1.0f, 1.0f,
		 x, -y,  0, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// textures
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Indicator::DrawIndicator(glm::mat4 projection, glm::mat4 view, bool menu)
{
	// background texture
	ResourceManager::GetShader("indShader").Use();
	ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("indShader").SetMatrix4("view", view);

	ResourceManager::GetShader("indShader").SetBool("menu", menu);
	ResourceManager::GetShader("indShader").SetBool("isImage", false);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	ResourceManager::GetShader("indShader").SetMatrix4("model", modelMatrix);
	ResourceManager::GetShader("indShader").SetVector3f("spriteColour", indColour);

	//glActiveTexture(GL_TEXTURE0);
	//indicator.texture.Bind();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//glBindTexture(GL_TEXTURE_2D, 0);

	// colour instance
	//ResourceManager::GetShader("indShader").Use();
	//ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	//ResourceManager::GetShader("indShader").SetMatrix4("view", view);

	//ResourceManager::GetShader("indShader").SetBool("menu", menu);
	//ResourceManager::GetShader("indShader").SetBool("isImage", false);

	//glm::mat4 modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, indicator.position);
}
