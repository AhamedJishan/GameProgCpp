#pragma once

#include "Component.h"
#include <GLEW/GL/glew.h>

namespace jLab
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int drawOrder = 100);
		~SpriteComponent();

		void Draw(class Shader* shader);
		void SetTexture(class Texture* texture);

		int GetDrawOrder() { return m_DrawOrder; }
		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		void InitSpriteVAO();

	private:
		GLuint m_SpriteVAO;
		GLuint m_VBO, m_EBO;

		class Texture* m_Texture;
		int m_Width;
		int m_Height;
		int m_DrawOrder;
	};
}