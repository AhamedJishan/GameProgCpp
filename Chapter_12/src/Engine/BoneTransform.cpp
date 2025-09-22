#include "BoneTransform.h"

namespace jLab
{
	glm::mat4 BoneTransform::ToMatrix() const
	{
		glm::mat4 ret = glm::scale(glm::mat4(1), Scale);
		ret = glm::mat4_cast(Rotation) * ret;
		ret = glm::translate(glm::mat4(1), Translation) * ret;
		return ret;
		//return glm::translate(glm::mat4(1.0f), Translation) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1), Scale);
	}

	BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float t)
	{
		BoneTransform boneTransform;
		boneTransform.Rotation = glm::slerp(a.Rotation, b.Rotation, t);
		boneTransform.Translation = glm::mix(a.Translation, b.Translation, t);
		boneTransform.Scale = glm::mix(a.Scale, b.Scale, t);
		return boneTransform;
	}

	BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float translationT, float rotationT, float scaleT)
	{
		BoneTransform boneTransform;
		boneTransform.Rotation = glm::slerp(a.Rotation, b.Rotation, rotationT);
		boneTransform.Translation = glm::mix(a.Translation, b.Translation, translationT);
		boneTransform.Scale = glm::mix(a.Scale, b.Scale, scaleT);
		return boneTransform;
	}
}