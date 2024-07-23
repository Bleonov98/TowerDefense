#include "Indicator.h"

Indicator::Indicator(glm::vec2 size)
{
    indSize = this->size = size;

    float x = size.x / 2.0f;
    float y = size.y / 2.0f;

    float vertices[] = {
        // Background vertices (with texture coordinates)
        -x,  y,  0, 0.0f, 1.0f,
         x, -y,  0, 1.0f, 0.0f,
        -x, -y,  0, 0.0f, 0.0f,

        -x,  y,  0, 0.0f, 1.0f,
         x,  y,  0, 1.0f, 1.0f,
         x, -y,  0, 1.0f, 0.0f,

         // HP indicator vertices (without texture coordinates)
         -x,  y,  0,
          x, -y,  0,
         -x, -y,  0,

         -x,  y,  0,
          x,  y,  0,
          x, -y,  0
    };

    // Generate VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind VAO for background
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Background vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // Background texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Generate and bind VAO for HP indicator
    glGenVertexArrays(1, &iVAO);
    glBindVertexArray(iVAO);
    // HP indicator vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(6 * 5 * sizeof(float)));
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Indicator::DrawIndicator(glm::mat4 projection, glm::mat4 view, bool menu)
{
	// background texture
	ResourceManager::GetShader("indShader").Use();
	ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("indShader").SetMatrix4("view", view);
	ResourceManager::GetShader("indShader").SetBool("menu", menu);
    
	ResourceManager::GetShader("indShader").SetBool("isImage", true);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	ResourceManager::GetShader("indShader").SetMatrix4("model", modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// hp indicator
	ResourceManager::GetShader("indShader").SetBool("isImage", false);
	ResourceManager::GetShader("indShader").SetVector3f("spriteColour", indColour);

    float x, y;
    y = indSize.y / 2.0f;
    x = indSize.x / 2.0f;

    float hpVertices[] = {
        -x,  y,  0,
         x, -y,  0,
        -x, -y,  0,

        -x,  y,  0,
         x,  y,  0,
         x, -y,  0
    };

    // Generate VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), sizeof(hpVertices), hpVertices);

    float percent = indSize.x / size.x;
    float displace = -size.x * (1.0f - percent);

	glm::mat4 hpMatrix = glm::mat4(1.0f);
	hpMatrix = glm::translate(hpMatrix, position + glm::vec3(displace / 2.0f, 0.0f, 0.01f));
	ResourceManager::GetShader("indShader").SetMatrix4("model", hpMatrix);

	glBindVertexArray(iVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
