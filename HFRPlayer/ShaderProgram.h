#pragma once
#include "Shader.h"
class ShaderProgram
{
	Shader* m_VertexShader;
	Shader* m_FragmentShader;

public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
	~ShaderProgram();

	bool SetAsCurrent();
	GLuint GetID() { return m_ProgId; }

private:
	bool m_VertexOK;
	bool m_FragmentOK;
	GLuint m_ProgId;

};

