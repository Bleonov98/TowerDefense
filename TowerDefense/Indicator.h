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

	glm::mat4 GetHPMatrix() { return hpMatrix; }
	glm::mat4 GetIndicatorMatrix() { return indMatrix; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetColour() { return indColour; }
	std::pair<glm::vec2, glm::vec2> GetSize() { return make_pair(size, indSize); }

	void RefreshData();
	void DrawIndicator(const std::vector<glm::mat4>& indicatorMatrices, const std::vector<glm::mat4>& hpMatrices, const std::vector<glm::vec3>& indColours, glm::mat4 projection, glm::mat4 view, bool menu);
	void DrawIndicatorsTexture(const std::vector<glm::mat4>& indicatorMatrices);
	void DrawIndicatorsHP(const std::vector<glm::mat4>& hpMatrices, const std::vector<glm::vec3>& indColours);

private:
	glm::mat4 indMatrix, hpMatrix;
	glm::vec3 position, indColour;
	glm::vec2 size, indSize;
	Texture2D texture;
	unsigned int VBO, matBuffer, VAO, iVAO;
};

#endif // !INDICATOR_H


