#ifndef HUD_H
#define HUD_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ResourceManager.h"

class HUD
{
public:

	HUD(unsigned int width, unsigned int height);

	void DrawHUD(bool menu);
	void AddTexture(Texture2D texture) { this->texture = texture; }

	~HUD() { glDeleteVertexArrays(1, &this->VAO); }

private:

	int width, height;
	unsigned int VAO, VBO;

	Texture2D texture;

	glm::mat4 projection;

};

#endif // !HUD_H



