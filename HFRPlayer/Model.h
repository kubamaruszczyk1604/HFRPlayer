#pragma once


// Include GLM
#include "GLM\GLM.hpp"
#include "GLM\GTC\matrix_transform.hpp"
#include "GLM\GTC\type_ptr.hpp"

#include "ShaderProgram.h"
#include "Mesh.h"

#include <iostream>


class Model
{

public:

	Model(ShaderProgram* shaderProgram, Mesh* mesh);
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	~Model();


	void SetTexture(GLuint texture) { m_Texture = texture; m_HasTexture = true; }
	GLuint GetTexture() { return m_Texture; }
	Mesh* GetMesh() { return p_Mesh; }
	ShaderProgram* GetShaderProgram() { return p_ShaderProgram; }

	void SetPosition(const glm::vec3& position) { m_Position = position; }
	void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
	void SetScale(const glm::vec3& scale) { m_Scale = scale; }

	const glm::vec3& GetPosition()const { return m_Position; }
	const glm::vec3& GetRotation()const { return m_Rotation; }
	const glm::vec3& GetScale()const { return m_Scale; }

	glm::vec3& GetPositionRef() { return m_Position; }
	glm::vec3& GetRotationRef() { return m_Rotation; }
	glm::vec3& GetScaleRef() { return m_Scale; }

	const glm::mat4& GetWorldMatrix();

	bool HasTexture()
	{
		//texturowany = 0;
		//std::cout << " MA TEXTURE " << m_HasTexture << std::endl;
		return m_HasTexture;
	}

private:

	bool m_HasTexture{ false };
	ShaderProgram* p_ShaderProgram;
	Mesh* p_Mesh;
	GLuint m_Texture{ 0 };

	glm::mat4 m_WorldMat{ 1.0f };
	glm::mat4 m_Identity{ 1.0f };
	glm::vec3 m_Position{ 0 };
	glm::vec3 m_Rotation{ 0 };
	glm::vec3 m_Scale{ glm::vec3(1,1,1) };


};