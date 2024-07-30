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
    glGenBuffers(1, &matBuffer);

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

void Indicator::RefreshData()
{
    // hp matrix
    float percent = indSize.x / size.x;
    float displace = -size.x * (1.0f - percent);

    hpMatrix = glm::mat4(1.0f);
    hpMatrix = glm::translate(hpMatrix, position + glm::vec3(displace / 2.0f, 0.0f, 0.01f));

    glBindVertexArray(iVAO);

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

    // Set new hp vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), sizeof(hpVertices), hpVertices);

    glBindVertexArray(0);

    // texture matrix 
    indMatrix = glm::mat4(1.0f);
    indMatrix = glm::translate(indMatrix, position);
}

void Indicator::DrawIndicator(const std::vector<glm::mat4>& indicatorMatrices, const std::vector<glm::mat4>& hpMatrices, const std::vector<glm::vec3>& indColours, const glm::mat4 projection, const glm::mat4 view, const bool menu)
{
    // setting shader
    ResourceManager::GetShader("indShader").Use();
    ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("indShader").SetMatrix4("view", view);
    ResourceManager::GetShader("indShader").SetBool("menu", menu);
    ResourceManager::GetShader("indShader").SetBool("instanced", true);

    DrawIndicatorsTexture(indicatorMatrices);
    DrawIndicatorsHP(hpMatrices, indColours);
}

void Indicator::DrawIndicatorsTexture(const std::vector<glm::mat4>& indicatorMatrices)
{
    // refresh matrix buffer
    glBindBuffer(GL_ARRAY_BUFFER, matBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * indicatorMatrices.size(), &indicatorMatrices.data()[0], GL_STATIC_DRAW);

    // matrix instructions for texture
    glBindVertexArray(VAO);

    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // indicator texture
    ResourceManager::GetShader("indShader").SetBool("isImage", true);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    // draw
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, indicatorMatrices.size());
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Indicator::DrawIndicatorsHP(const std::vector<glm::mat4>& hpMatrices, const std::vector<glm::vec3>& indColours)
{
    ResourceManager::GetShader("indShader").SetBool("isImage", false);

    glBindVertexArray(iVAO);

    //float x, y;
    //y = indSize.y / 2.0f;
    //x = indSize.x / 2.0f;

    //float hpVertices[] = {
    //    -x,  y,  0,
    //     x, -y,  0,
    //    -x, -y,  0,

    //    -x,  y,  0,
    //     x,  y,  0,
    //     x, -y,  0
    //};

    //// Set new hp vertices
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferSubData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), sizeof(hpVertices), hpVertices);

    // refresh colour buffer
    unsigned int colourVBO;
    glGenBuffers(1, &colourVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indColours.size(), &indColours.data()[0], GL_STATIC_DRAW);

    // colour instructions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glVertexAttribDivisor(2, 1);

    // refresh matrix buffer
    glBindBuffer(GL_ARRAY_BUFFER, matBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * hpMatrices.size(), &hpMatrices.data()[0], GL_STATIC_DRAW);

    // matrix instructions for colour
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // draw
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, hpMatrices.size());
    glBindVertexArray(0);
}

void Indicator::DrawIndicatorsHP(const glm::mat4 projection, const glm::mat4 view, const bool menu)
{
    ResourceManager::GetShader("indShader").Use();
    ResourceManager::GetShader("indShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("indShader").SetMatrix4("view", view);
    ResourceManager::GetShader("indShader").SetBool("menu", menu);

    ResourceManager::GetShader("indShader").SetBool("isImage", false);
    ResourceManager::GetShader("indShader").SetBool("instanced", false);
    ResourceManager::GetShader("indShader").SetMatrix4("uModel", hpMatrix);

    glBindVertexArray(iVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
