#pragma once
#include "GLM\GLM.hpp"


struct Vertex
{

	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_UVs;

	Vertex()
	{
		m_Position = glm::vec3(0, 0, 0);
		m_Normal = glm::vec3(0, 1, 0);
		m_UVs = glm::vec2(0, 0);
	}

	Vertex(const glm::vec3& pos)
	{
		m_Position = pos;
	}

	Vertex(float x, float y, float z)
	{
		m_Position = glm::vec3(x, y, z);
	}


	Vertex(const glm::vec3& pos, const glm::vec3& nor)
	{
		m_Position = pos;
		m_Normal = nor;
		m_UVs = glm::vec2(0, 0);
	}

	Vertex(const glm::vec3& pos, const glm::vec3& nor, const glm::vec2& uv)
	{
		m_Position = pos;
		m_Normal = nor;
		m_UVs = uv;
	}



	void SetXYZ(const glm::vec3& pos)
	{
		m_Position = pos;
	}
	void SetNormal(const glm::vec3& nor)
	{
		m_Normal = nor;
	}

	void SetUV(float u, float v)
	{
		m_UVs = glm::vec2(u, v);
	}


	glm::vec3 GetXYZ()
	{
		return m_Position;
	}

	glm::vec3 GetNormal()
	{
		return m_Normal;
	}

	glm::vec2 GetUV()
	{
		return m_UVs;
	}


};
