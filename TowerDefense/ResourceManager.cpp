#include "ResourceManager.h"

std::map<std::string, Model> ResourceManager::models;

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

// Shaders
Shader ResourceManager::LoadShader(const char* vShader, const char* fShader, std::string name)
{
    shaders[name] = LoadShaderFromFile(vShader, fShader);
    return shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return shaders[name];
}

// Model
Model ResourceManager::LoadModel(const char* fileName, std::string name)
{
    std::string filePath = "../models/";
    filePath += fileName;

    models[name] = LoadModelFromFile(filePath, name);
    return models[name];
}

Model ResourceManager::GetModel(std::string name)
{
    return models[name];
}

// Texture2D
Texture2D ResourceManager::LoadTexture(const char* fileName, bool alpha, std::string name)
{
    std::string filePath = "../textures/";
    filePath += fileName;

    textures[name] = LoadTextureFromFile(filePath.c_str(), alpha);
    return textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return textures[name];
}

// Loading
Model ResourceManager::LoadModelFromFile(std::string& const fileName, std::string name)
{
    Model model(fileName, name);

    try
    {
        Animation anim(fileName, &model);
        Animator animator(&anim);

        model.Animated();
    }
    catch (const bool ex) {}

    return model;
}

Shader ResourceManager::LoadShaderFromFile(const char* vShader, const char* fShader)
{
    Shader shader;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vShader);
        fShaderFile.open(fShader);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int sVertex, sFragment;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vShaderCode, NULL);
    glCompileShader(sVertex);
    shader.checkCompileErrors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fShaderCode, NULL);
    glCompileShader(sFragment);
    shader.checkCompileErrors(sFragment, "FRAGMENT");
    // shader program
    shader.GetID() = glCreateProgram();
    glAttachShader(shader.GetID(), sVertex);
    glAttachShader(shader.GetID(), sFragment);
    glLinkProgram(shader.GetID());
    shader.checkCompileErrors(shader.GetID(), "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);

    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* fileName, bool alpha)
{
    Texture2D txt;

    if (alpha)
    {
        txt.SetAlpha(true);
        txt.SetFormatAlpha();
    }
    // load image
    int Width, Height, nrChannels;
    unsigned char* data = stbi_load(fileName, &Width, &Height, &nrChannels, 0);

    txt.Generate(Width, Height, data);

    stbi_image_free(data);

    return txt;
}