#pragma once
#include "GLM\GLM.hpp"
struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;

	Vertex(const glm::vec3& pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}
	Vertex()
	{
		x = 0; y = 0; z = 0;
		nx = 0; ny = 0; nz = 0;
		u = 0; v = 0;
	}

	Vertex(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vertex(const glm::vec3& pos, const glm::vec3& nor)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;

		nx = nor.x;
		ny = nor.y;
		nz = nor.z;
	}

	Vertex(const glm::vec3& pos, const glm::vec3& nor, const glm::vec2& uv)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;

		nx = nor.x;
		ny = nor.y;
		nz = nor.z;

		u = uv.x;
		v = uv.y;
	}



	void SetXYZ(const glm::vec3& pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}
	void SetNormal(const glm::vec3& nor)
	{
		nx = nor.x;
		ny = nor.y;
		nz = nor.z;
	}

	void SetUV(float U, float V)
	{
		u = U;
		v = V;
	}


	glm::vec3 GetXYZ()
	{
		return glm::vec3(x, y, z);
	}

	glm::vec3 GetNormal()
	{
		return glm::vec3(nx, ny, nz);
	}

	glm::vec2 GetUV()
	{
		return glm::vec2(u, v);
	}


};
