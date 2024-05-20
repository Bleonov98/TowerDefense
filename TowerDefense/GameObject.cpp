#include "GameObject.h"

void GameObject::RefreshMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	objMatrix = modelMatrix;
}

void GameObject::SetPosition(glm::vec3 pos)
{
	model.TranslateModel(pos - this->position);
	this->position = pos;
}

void GameObject::SetScale(glm::vec3 scale)
{
	model.ScaleModel(scale);
	this->scale = scale;
}

void GameObject::SetAngle(float angle)
{
	model.RotateModel(angle - this->angle);
	this->angle = angle;
}

void GameObject::SetModel(Model model)
{
    this->model = model;
    RefreshModel();
}

void GameObject::RefreshModel()
{
    this->model.TranslateModel(position);
    this->model.ScaleModel(scale);
    this->model.RotateModel(angle);
}

std::string vec3ToString(const glm::vec3& vec) {
    std::ostringstream oss;
    oss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
}

bool GameObject::RayCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
{
    bool hit = false;
    float closestT = FLT_MAX;

    glm::vec3 normRayDirection = glm::normalize(rayDirection);

    for (size_t i = 0; i < model.meshes.size(); ++i)
    {
        const auto& mesh = model.meshes[i];

        for (size_t j = 0; j < mesh.indices.size(); j += 3)
        {
            unsigned int index0 = mesh.indices[j];
            unsigned int index1 = mesh.indices[j + 1];
            unsigned int index2 = mesh.indices[j + 2];

            glm::vec3 v0 = mesh.vertices[index0].Position;
            glm::vec3 v1 = mesh.vertices[index1].Position;
            glm::vec3 v2 = mesh.vertices[index2].Position;

            // std::cout << "Triangle vertices: " << vec3ToString(v0) << ", " << vec3ToString(v1) << ", " << vec3ToString(v2) << std::endl;

            float t;
            if (RayIntersectsTriangle(rayOrigin, normRayDirection, v0, v1, v2, t))
            {
                if (t < closestT)
                {
                    closestT = t;
                    hit = true;
                }
            }
        }
    }

    return hit;
}

bool GameObject::RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2, float& outIntersectionDistance)
{
    const float EPSILON = 0.0000001f;
    glm::vec3 edge1 = vertex1 - vertex0;
    glm::vec3 edge2 = vertex2 - vertex0;
    glm::vec3 h = glm::cross(rayVector, edge2);
    float a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON) {
        // std::cout << "Ray is parallel to the triangle" << std::endl;
        return false;
    }

    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - vertex0;
    float u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        // std::cout << "U out of range: " << u << std::endl;
        // std::cout << "rayOrigin: " << vec3ToString(rayOrigin) << ", rayVector: " << vec3ToString(rayVector) << std::endl;
        // std::cout << "vertex0: " << vec3ToString(vertex0) << ", vertex1: " << vec3ToString(vertex1) << ", vertex2: " << vec3ToString(vertex2) << std::endl;
        return false;
    }

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayVector, q);

    if (v < 0.0f || u + v > 1.0f) {
        // std::cout << "V out of range: " << v << std::endl;
        // std::cout << "rayOrigin: " << vec3ToString(rayOrigin) << ", rayVector: " << vec3ToString(rayVector) << std::endl;
        // std::cout << "vertex0: " << vec3ToString(vertex0) << ", vertex1: " << vec3ToString(vertex1) << ", vertex2: " << vec3ToString(vertex2) << std::endl;
        return false;
    }

    float t = f * glm::dot(edge2, q);

    if (t > EPSILON) {
        outIntersectionDistance = t;
        return true;
    }
    else {
        // std::cout << "Intersection behind the ray origin" << std::endl;
        return false; 
    }
}


void GameObject::DrawObject()
{
	Shader shader = ResourceManager::GetShader("modelShader");
	model.Draw(shader);
}
