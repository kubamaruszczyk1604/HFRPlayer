#include "stdafx.h"
#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, ShaderStringType type) :
	m_VertexOK(false),
	m_FragmentOK(false)
{
	m_VertexShader = new Shader(GL_VERTEX_SHADER);
	bool success = true;
	if (type == ShaderStringType::Path) m_VertexShader->LoadFromFile(vertexShader);
	else m_VertexShader->SetShaderString(vertexShader);
	if (!success)
	{
		std::cerr << "Failed to load vertex shader " << std::endl;
		return;
	}
	std::string log;
	m_VertexOK = m_VertexShader->Compile(log);
	std::cout << "Vertex Shader status: " << log << std::endl;



	m_FragmentShader = new Shader(GL_FRAGMENT_SHADER);

	success = true;
	if (type == ShaderStringType::Path) m_FragmentShader->LoadFromFile(fragmentShader);
	else m_FragmentShader->SetShaderString(fragmentShader);
	if (!success)
	{
		std::cerr << "Failed to load fragment shader " << std::endl;

		return;
	}
	m_FragmentOK = m_FragmentShader->Compile(log);
	std::cout << "Fragment Shader status: " << log << std::endl;

	m_ProgId = glCreateProgram();
	glAttachShader(m_ProgId, m_VertexShader->GetID());
	glAttachShader(m_ProgId, m_FragmentShader->GetID());

	glLinkProgram(m_ProgId);


}


ShaderProgram::~ShaderProgram()
{
	glDetachShader(m_ProgId, m_VertexShader->GetID());
	glDetachShader(m_ProgId, m_FragmentShader->GetID());

	delete m_FragmentShader;
	delete m_VertexShader;

	glDeleteProgram(m_ProgId);

}

bool ShaderProgram::SetAsCurrent()
{
	if (m_VertexOK&&m_FragmentOK)
	{
		glUseProgram(m_ProgId);

		return true;
	}
	return false;
}
