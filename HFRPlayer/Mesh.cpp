#include "Mesh.h"
#include <GLFW/glfw3.h>  

Mesh::Mesh() :ptr_VBO{ 0,0 }, ptr_VAO{ 0 }, m_IndexCount{ 0 }
{
}


Mesh::~Mesh()
{
}

bool Mesh::Create(Vertex vertices[], int numVertices)
{
	//VBO
	glGenBuffers(2, ptr_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ptr_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//VAO
	glGenVertexArrays(1, &ptr_VAO);
	glBindVertexArray(ptr_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ptr_VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(3 * sizeof(float)));

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //uv

	m_VertexCount = numVertices;
	return true;
}

void Mesh::Draw() const
{
	glBindVertexArray(ptr_VAO);
	if (m_IndexCount == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	}
}

void Mesh::CreateIndexBuffer(int indices[], int numIndices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ptr_VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(int), &indices[0], GL_STATIC_DRAW);
	m_IndexCount = numIndices;
	m_indicesPtr = indices;
}


