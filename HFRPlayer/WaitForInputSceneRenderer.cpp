#include "WaitForInputSceneRenderer.h"

#include "GLTextureLoader.h"



WaitForInputSceneRenderer::WaitForInputSceneRenderer()
{
	initialiseShader();
	m_ReadyScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/player_ready.png");
}


WaitForInputSceneRenderer::~WaitForInputSceneRenderer()
{
	glDeleteTextures(1, &m_ReadyScrTexID);
}

void WaitForInputSceneRenderer::render()
{
	m_shader->SetAsCurrent();

	GLuint loct = glGetUniformLocation(m_shader->GetID(), "time");
	glUniform1f(loct, s_GlobalClock.ElapsedTime() * 1.5);
	GLuint locc = glGetUniformLocation(m_shader->GetID(), "col");
	const float col[] = { 1.0,1.0,1.0 };
	glUniform3f(locc, 0.41, 0.41, 0);
	glBindTexture(GL_TEXTURE_2D, m_ReadyScrTexID);
	s_QuadMesh->Draw();
}

void WaitForInputSceneRenderer::reset()
{
}

std::string WaitForInputSceneRenderer::genFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform vec3 col;\n\n";
	fragmentShader += "uniform float time;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "vec4 sampled = texture2D(SCT_TEXTURE2D_0,oUVs); \n";
	fragmentShader += "if((sampled.x + sampled.y + sampled.z) < 2.6) \n";
	fragmentShader += "FragColour =  sampled - (0.5*abs(sin(time))*vec4(col,1)); \n";
	fragmentShader += "else FragColour =  sampled; \n}\n";
	return fragmentShader;
}
