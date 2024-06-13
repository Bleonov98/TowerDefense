#ifndef INDICATOR_H
#define INDICATOR_H

#include "GameObject.h"

class Indicator
{
public:
	Indicator() {};
	Indicator(glm::vec2 size);

	void AddTexture(Texture2D texture) { this->texture = texture; }

	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetColour(glm::vec3 colour) { this->indColour = colour; }
	void SetSize(glm::vec2 size) { this->indSize = size; }

	glm::vec3 GetPosition() { return position; }
	std::pair<glm::vec2, glm::vec2> GetSize() { return make_pair(size, indSize); }

	void DrawIndicator(glm::mat4 projection, glm::mat4 view, bool menu);

private:
	glm::vec3 position, indColour;
	glm::vec2 size, indSize;
	Texture2D texture;
	unsigned int VBO, VAO, iVAO;
};

#endif // !INDICATOR_H


