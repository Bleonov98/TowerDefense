#ifndef RESOURCE_H
#define RESOURCE_H

#include "Shader.h"
#include "Model.h"

#include <string>
#include <map>

class ResourceManager
{
public:

	// shaders
	static Shader LoadShader(const char* vShader, const char* fShader, std::string name);

	static Shader GetShader(std::string name);

	// textures
	static Model LoadModel(const char* fileName, bool alpha, std::string name);

	static Model GetModel(std::string name);

private:

	ResourceManager() {};

	static std::map<std::string, Model> models;
	static std::map<std::string, Shader> shaders;

	static Model LoadModelFromFile(std::string& const fileName, bool alpha);
	static Shader LoadShaderFromFile(const char* vShader, const char* fShader);

};


#endif // !RESOURCE_H