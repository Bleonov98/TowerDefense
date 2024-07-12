#ifndef RESOURCE_H
#define RESOURCE_H

#include "Shader.h"
#include "Model.h"
#include "Texture2D.h"
#include "Animation.h"
#include "Animator.h"

#include <string>
#include <map>

class ResourceManager
{
public:

	// shaders
	static Shader LoadShader(const char* vShader, const char* fShader, std::string name);
	static Shader GetShader(std::string name);

	// models
	static Model LoadModel(const char* fileName, std::string name);
	static Model GetModel(std::string name);
	// static Animation GetAnimation(std::string name);

	// Textures
	static Texture2D LoadTexture(const char* fileName, bool alpha, std::string name);
	static Texture2D GetTexture(std::string name);

private:

	ResourceManager() {};

	static std::map<std::string, Model> models;
	// static std::map<std::string, Animation> animations;

	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Texture2D LoadTextureFromFile(const char* fileName, bool alpha);
	static Model LoadModelFromFile(std::string& const fileName, std::string name);
	static Shader LoadShaderFromFile(const char* vShader, const char* fShader);

};


#endif // !RESOURCE_H