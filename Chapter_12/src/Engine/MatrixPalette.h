#pragma once

#include <glm/mat4x4.hpp>

namespace jLab
{
	const int MAX_SKELETON_BONES = 96;
	struct MatrixPalette
	{
		glm::mat4 m_Entry[MAX_SKELETON_BONES];
	};
}