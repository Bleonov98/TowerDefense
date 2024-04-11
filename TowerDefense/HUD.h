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
	void DrawHUD(glm::vec2 pos, glm::vec2 size, bool menu);

	void AddTexture(Texture2D texture) { this->texture = texture; }

	~HUD() { glDeleteVertexArrays(1, &this->VAO); }

protected:

	int width, height;
	unsigned int VAO, VBO;

	Texture2D texture;

	glm::mat4 projection;

};

enum ButtonType {
	BOW,
	FIRE,
	ICE
};

class Button : public HUD
{
public:

	Button(glm::vec2 position, glm::vec2 size, ButtonType type, unsigned int width, unsigned int height) : HUD(width, height) {
		this->size = size;
		this->position = position;
		this->type = type;
	};

	glm::vec2 GetButtonPosition() { return this->position; }
	glm::vec2 GetButtonSize() { return this->size; }

	void SetButtonPosition(glm::vec2 position) { this->position = position; }
	void SetButtonSize(glm::vec2 size) { this->size = size; }

	void DrawButton(bool menu);

	bool ButtonCollision(glm::vec2 clickPos);

private:

	glm::vec2 position, size;
	ButtonType type = BOW;

};

#endif // !HUD_H