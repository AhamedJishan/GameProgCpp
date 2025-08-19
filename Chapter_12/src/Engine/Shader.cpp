#include "Shader.h"

#include <fstream>
#include <sstream>
#include <SDL/SDL.h>
#include <glm/gtc/type_ptr.hpp>

namespace jLab
{
	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
		:m_Id(-1)
	{
		Load(vertexShader, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Id);
	}

	void Shader::SetActive()
	{
		glUseProgram(m_Id);
	}

	bool Shader::Load(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vert, frag;

		if (!Compile(vertexShader, GL_VERTEX_SHADER, vert) || !Compile(fragmentShader, GL_FRAGMENT_SHADER, frag))
			return false;

		m_Id = glCreateProgram();
		glAttachShader(m_Id, vert);
		glAttachShader(m_Id, frag);
		glLinkProgram(m_Id);

		if (!IsProgramValid())
			return false;

		glDeleteShader(vert);
		glDeleteShader(frag);

		return true;
	}

	bool Shader::Compile(const std::string& shaderSource, GLenum shaderType, unsigned int& outShader)
	{
		std::ifstream shaderFile(shaderSource);

		if (shaderFile.is_open())
		{
			std::stringstream contentStream;
			contentStream << shaderFile.rdbuf();
			std::string contentString = contentStream.str();
			const char* contentData = contentString.c_str();

			outShader = glCreateShader(shaderType);
			glShaderSource(outShader, 1, &contentData, nullptr);
			glCompileShader(outShader);
			if (!IsCompiled(outShader))
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile shader : %s", shaderSource.c_str());
				return false;
			}
		}
		else
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open shader file : %s", shaderSource.c_str());
			return false;
		}

		return true;
	}

	bool Shader::IsCompiled(unsigned int shader)
	{
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetShaderInfoLog(shader, 511, nullptr, buffer);
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLSL Compilation failed :- \n%s", buffer);
			return false;
		}

		return true;
	}

	bool Shader::IsProgramValid()
	{
		GLint status;
		glGetProgramiv(m_Id, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetProgramInfoLog(m_Id, 511, nullptr, buffer);
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLSL Program link failed :- \n%s", buffer);
			return false;
		}

		return true;
	}

	// ------------------UNIFORM util functions-------------------------
	void Shader::SetBool(const std::string& name, const bool value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
	}
	void Shader::SetInt(const std::string& name, const int value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	void Shader::SetFloat(const std::string& name, const float value) const
	{
		glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	void Shader::SetVec3(const std::string& name, const float v1, const float v2, const float v3) const
	{
		glUniform3f(glGetUniformLocation(m_Id, name.c_str()), v1, v2, v3);
	}
	void Shader::SetVec3(const std::string& name, const glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y, value.z);
	}
	void Shader::SetVec2(const std::string& name, const float v1, const float v2) const
	{
		glUniform2f(glGetUniformLocation(m_Id, name.c_str()), v1, v2);
	}
	void Shader::SetVec2(const std::string& name, const glm::vec2 value) const
	{
		glUniform2f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y);
	}
	void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}