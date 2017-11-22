#pragma once
#include "Vertex.h"
//Include GLEW  
#include <GL/glew.h>  

//Include GLFW  
#include <GLFW/glfw3.h>  
class Mesh
{
public:
	Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	~Mesh();

	bool Create(Vertex vertices[], int numVertices);
	void Draw()const;
	void CreateIndexBuffer(int indices[], int numIndices);

private:
	GLuint ptr_VBO[2];
	GLuint ptr_VAO;
	int m_VertexCount;
	int m_IndexCount;
	int *m_indicesPtr;
};

