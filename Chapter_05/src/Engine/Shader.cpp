#include "Shader.h"

#include <fstream>
#include <sstream>
#include <SDL/SDL.h>

namespace jLab
{
	Shader::Shader()
		:m_ShaderProgram(0),
		m_VertexShader(0),
		m_FragmentShader(0)
	{
	}
	
	Shader::~Shader()
	{
	}
	
	bool Shader::Load(const std::string& vertexShader, const std::string& fragmentShader)
	{
		if (!CompileShader(vertexShader, GL_VERTEX_SHADER, m_VertexShader) ||
			!CompileShader(fragmentShader, GL_FRAGMENT_SHADER, m_FragmentShader))
		{
			return false;
		}

		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, m_VertexShader);
		glAttachShader(m_ShaderProgram, m_FragmentShader);
		glLinkProgram(m_ShaderProgram);

		if (!IsProgramValid(m_ShaderProgram))
			return false;

		return true;
	}
	
	void Shader::Unload()
	{
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
		glDeleteProgram(m_ShaderProgram);
	}
	
	void Shader::SetActive()
	{
		glLinkProgram(m_ShaderProgram);
	}
	
	bool Shader::CompileShader(const std::string& filename, GLenum shaderType, GLuint& outShader)
	{
		std::ifstream shaderFile(filename);

		if (shaderFile.is_open())
		{
			std::stringstream sstream;
			sstream << shaderFile.rdbuf();
			const char* contents = sstream.str().c_str();

			outShader = glCreateShader(shaderType);
			glShaderSource(outShader, 1, &contents, nullptr);
			glCompileShader(outShader);

			if (!IsCompiled(outShader))
			{
				SDL_Log("Failed to compile shader: %s", filename.c_str());
				return false;
			}
		}
		else
		{
			SDL_Log("Shader File not found: %s", filename.c_str());
			return false;
		}
		return true;
	}
	
	bool Shader::IsCompiled(GLuint shader)
	{
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetShaderInfoLog(shader, 511, nullptr, buffer);
			SDL_Log("GLSL Compilation failed: %s", buffer);
			return false;
		}

		return true;
	}
	
	bool Shader::IsProgramValid(GLuint program)
	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetProgramInfoLog(program, 511, nullptr, buffer);
			SDL_Log("GLSL linking failed: %s", buffer);
			return false;
		}

		return true;
	}
}