#include "stdafx.h"
#include "Model.h"


Model::Model(ShaderProgram* shaderProgram, Mesh* mesh) :
	p_ShaderProgram{ shaderProgram },
	p_Mesh{ mesh }
{
}


Model::~Model()
{
}

const glm::mat4& Model::GetWorldMatrix()
{
	m_WorldMat = glm::translate(m_Identity, m_Position);
	m_WorldMat = glm::rotate(m_WorldMat, m_Rotation.x, glm::vec3(1, 0, 0));
	m_WorldMat = glm::rotate(m_WorldMat, m_Rotation.y, glm::vec3(0, 1, 0));
	m_WorldMat = glm::rotate(m_WorldMat, m_Rotation.z, glm::vec3(0, 0, 1));

	m_WorldMat = glm::scale(m_WorldMat, m_Scale);
	return m_WorldMat;
}
