#include "HUD.h"

HUD::HUD(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;

	ResourceManager::LoadShader("../shaders/vHudShader.vx", "../shaders/fHudShader.ft", "hudShader");
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HUD::DrawHUD(bool menu)
{
    ResourceManager::GetShader("hudShader").Use();
    ResourceManager::GetShader("hudShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("hudShader").SetVector3f("spriteColor", glm::vec3(1.0f));

    ResourceManager::GetShader("hudShader").SetBool("menu", menu);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void HUD::DrawHUD(glm::vec2 pos, glm::vec2 size, bool menu)
{
    ResourceManager::GetShader("hudShader").Use();
    ResourceManager::GetShader("hudShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("hudShader").SetVector3f("spriteColor", glm::vec3(1.0f));

    ResourceManager::GetShader("hudShader").SetBool("menu", menu);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Button::DrawButton(bool menu)
{
    ResourceManager::GetShader("hudShader").Use();
    ResourceManager::GetShader("hudShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("hudShader").SetVector3f("spriteColor", glm::vec3(1.0f));

    ResourceManager::GetShader("hudShader").SetBool("menu", menu);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Button::ButtonCollision(glm::vec2 clickPos)
{
    glm::vec2 minPoint, maxPoint;

    minPoint = position;
    maxPoint = position + size;

    if (clickPos.x >= minPoint.x && clickPos.x <= maxPoint.x &&
        clickPos.y >= minPoint.y && clickPos.y <= maxPoint.y) return true;

    return false;
}
