#include "ISceneRenderer.h"

#include <sstream>

Stopwatch ISceneRenderer::s_GlobalClock;
Mesh* ISceneRenderer::s_QuadMesh = nullptr;


void ISceneRenderer::initialise()
{
	Vertex vertices[] =
	{
		//GL UVs ORDERING
		Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0)),
		Vertex(glm::vec3(1, -1, 0), glm::vec2(1,0)),
		Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1))
	};

	s_QuadMesh = new Mesh();
	s_QuadMesh->Create(vertices, 4);

	int indices[] = { 0,1,2,0,2,3 };
	s_QuadMesh->CreateIndexBuffer(indices, 6);

	s_GlobalClock.Start();
}

void ISceneRenderer::cleanup()
{
	delete s_QuadMesh;
	s_GlobalClock.Stop();
}

std::string ISceneRenderer::genDefaultVertexShader()
{
	std::ostringstream s;
	s <<  "#version 330\n\n";
	s << "layout(location = 0) in vec3 vertex_position;\n";
	s << "layout(location = 1) in vec2 uvs;\n\n";
	s << "out vec2 oUVs;\n\n";
	s << "void main()\n{\n";
	s << "  oUVs = uvs;\n";
	s << "   gl_Position =  vec4(vertex_position,1.0);\n}";
	return s.str();
}

void ISceneRenderer::initialiseShader()
{
	m_shader = new ShaderProgram(genDefaultVertexShader(), genFragShader(), ShaderStringType::Content);
	GLuint samplerID = glGetUniformLocation(m_shader->GetID(), "SCT_TEXTURE2D_0");
	glUniform1i(samplerID, 0);
	glActiveTexture(GL_TEXTURE0);
}
