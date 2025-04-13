#pragma once

#include <string>
#include "GLEW/GL/glew.h"

namespace jLab
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		bool Load(const std::string& vertexShader, const std::string& fragmentShader);
		void Unload();

		void SetActive();

	private:
		bool CompileShader(const std::string& filename, GLenum shaderType, GLuint& outShader);
		
		bool IsCompiled(GLuint shader);
		bool IsProgramValid(GLuint program);

	private:
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_ShaderProgram;
	};
}