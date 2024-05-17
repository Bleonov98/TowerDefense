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
            glm::vec3 v0 = mesh.vertices[i].Position;
            glm::vec3 v1 = mesh.vertices[i + 1].Position;
            glm::vec3 v2 = mesh.vertices[i + 2].Position;

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

bool GameObject::RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t)
{
    const float EPSILON = 0.0000001f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(rayDirection, edge2);
    float a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;

    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDirection, q);
    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * glm::dot(edge2, q);
    if (t > EPSILON)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameObject::DrawObject()
{
	Shader shader = ResourceManager::GetShader("modelShader");
	model.Draw(shader);
}
