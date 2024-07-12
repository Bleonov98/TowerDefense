#ifndef ANIMATOR_CLASS_H
#define ANIMATOR_CLASS_H

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Animation.h"
#include "Bone.h"

class Animator
{
public:

	Animator() {};

	Animator(Animation* animation);

	void UpdateAnimation(float dt);

	void PlayAnimation(Animation* pAnimation);

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }

private:
	Animation* m_CurrentAnimation;
	std::vector<glm::mat4> m_FinalBoneMatrices;
	float m_CurrentTime;
	float m_DeltaTime;

};

#endif // !ANIMATOR_CLASS_H