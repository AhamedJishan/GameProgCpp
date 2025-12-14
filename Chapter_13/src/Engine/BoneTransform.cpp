#include "BoneTransform.h"

namespace jLab
{
	glm::mat4 BoneTransform::ToMatrix() const
	{
		glm::mat4 ret;
		ret = glm::scale(glm::mat4(1), Scale);
		ret = glm::mat4_cast(Rotation) * ret;
		ret = glm::translate(glm::mat4(1), Position) * ret;
		return ret;
	}

	BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float t)
	{
		BoneTransform ret;
		ret.Scale = glm::mix(a.Scale, b.Scale, t);
		ret.Rotation = glm::slerp(a.Rotation, b.Rotation, t);
		ret.Position = glm::mix(a.Position, b.Position, t);
		return ret;
	}
	
	BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float translationT, float rotationT, float scaleT)
	{
		BoneTransform ret;
		ret.Scale = glm::mix(a.Scale, b.Scale, scaleT);
		ret.Rotation = glm::slerp(a.Rotation, b.Rotation, rotationT);
		ret.Position = glm::mix(a.Position, b.Position, translationT);
		return ret;
	}
}