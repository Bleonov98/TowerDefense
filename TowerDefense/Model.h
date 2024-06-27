#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "Mesh.h"
#include "AssimpGlmHelpers.h"
#include "Animdata.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    Model() {};
    // constructor, expects a filepath to a 3D model.
    Model(string const& path, string name, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader& shader);

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }
    
    // calculations
    void CalculateSize();

    void TranslateModel(glm::vec3 newPosition);
    void ScaleModel(glm::vec3 scale, glm::vec3 position);
    void RotateModel(float rotate);

    // properties
    glm::vec3 GetSize() { return modelSize; }

    std::string GetName() { return this->name; }
    std::string GetPath() { return this->filePath; }

    void Animated() { animated = true; }
    bool IsAnimated() { return animated; }

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

    void SetVertexBoneDataToDefault(Vertex& vertex);

    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    // model properties 
    glm::vec3 modelSize;
    bool animated = false;
    std::string name, filePath;

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;
};

#endif