#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <GLEW/GL/glew.h>

namespace jLab
{
	class Shader
	{
	public:
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		~Shader();

		void SetActive();

		void SetBool(const std::string& name, const bool value) const;
		void SetInt(const std::string& name, const int value) const;
		void SetFloat(const std::string& name, const float value) const;
		void SetVec3(const std::string& name, const float v1, const float v2, const float v3) const;
		void SetVec3(const std::string& name, const glm::vec3 value) const;
		void SetVec2(const std::string& name, const float v1, const float v2) const;
		void SetVec2(const std::string& name, const glm::vec2 value) const;
		void SetMat4(const std::string& name, const glm::mat4& mat) const;

	private:
		bool Load(const std::string& vertexShader, const std::string& fragmentShader);
		bool Compile(const std::string& shaderSource, GLenum shaderType, unsigned int& outShader);
		bool IsCompiled(unsigned int shader);
		bool IsProgramValid();

	private:
		unsigned int m_Id;
	};
}