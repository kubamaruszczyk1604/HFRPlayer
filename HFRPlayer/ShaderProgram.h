#pragma once
#include "Shader.h"

enum class ShaderStringType { Path, Content };

class ShaderProgram
{
	Shader* m_VertexShader;
	Shader* m_FragmentShader;

public:
	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, ShaderStringType type);
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	~ShaderProgram();

	bool SetAsCurrent();
	GLuint GetID() { return m_ProgId; }

private:
	bool m_VertexOK;
	bool m_FragmentOK;
	GLuint m_ProgId;

};

