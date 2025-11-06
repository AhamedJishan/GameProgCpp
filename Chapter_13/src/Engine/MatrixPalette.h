#pragma once

#include <glm/mat4x4.hpp>

namespace jLab
{
	const int MAX_NUM_BONES = 96;

	struct MatrixPalette
	{
		glm::mat4 mEntry[MAX_NUM_BONES];
	};
}