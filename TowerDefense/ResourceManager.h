#ifndef RESOURCE_H
#define RESOURCE_H

#include "Shader.h"
#include "Model.h"
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
	static Animator GetAnimator(std::string name);
	static Animation GetAnimation(std::string name);

private:

	ResourceManager() {};

	static std::map<std::string, Model> models;
	static std::map<std::string, Animation> animations;
	static std::map<std::string, Animator> animators;

	static std::map<std::string, Shader> shaders;

	static Model LoadModelFromFile(std::string& const fileName, std::string name);
	static Shader LoadShaderFromFile(const char* vShader, const char* fShader);

};


#endif // !RESOURCE_H