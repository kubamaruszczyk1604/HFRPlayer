#include "stdafx.h"
#include "Shader.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

//#define PRINT_SHADERS

Shader::Shader(GLenum type) :m_Id(0)
{
	m_Id = glCreateShader(type);
}


Shader::~Shader()
{
	glDeleteShader(m_Id);
}

bool Shader::LoadFromFile(const std::string & path)
{
	std::ifstream ifs(path);

	if (!ifs.is_open()) return false;
	std::string result;
	char buffer;
	while (ifs.get(buffer))
	{
		result += buffer;
	}

#ifdef PRINT_SHADERS
	std::cout << result << std::endl;
#endif
	const char * c = result.c_str();
	glShaderSource(m_Id, 1, &c, NULL);
	return true;
}

void Shader::SetShaderString(const std::string & content)
{
#ifdef PRINT_SHADERS
	std::cout << content << std::endl;
#endif
	const char * c = content.c_str();
	glShaderSource(m_Id, 1, &c, NULL);
}

bool Shader::Compile(std::string& log)
{
	glCompileShader(m_Id);
	GLint isCompiled = 0;
	glGetShaderiv(m_Id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_Id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(m_Id, maxLength, &maxLength, &errorLog[0]);
		log = std::string(errorLog.begin(), errorLog.end());
		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(m_Id); // Don't leak the shader.
		return false;
	}
	else
	{
		log = "NO ERRORS";
		return true;
	}
}

GLuint Shader::GetID()
{
	return m_Id;
}


