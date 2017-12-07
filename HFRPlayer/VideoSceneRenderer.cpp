#include "VideoSceneRenderer.h"

#include <algorithm>

VideoSceneRenderer::VideoSceneRenderer(std::vector<GLuint>* images) :
	IVideoSceneRenderer(images)
{
	initialiseShader();
	m_alphaLoc = glGetUniformLocation(m_shader->GetID(), "alpha");
}

VideoSceneRenderer::~VideoSceneRenderer()
{
}

void VideoSceneRenderer::render()
{
	m_shader->SetAsCurrent();
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_CurrentIndex]);
	glUniform1f(m_alphaLoc, std::pow(std::min(1., std::max(0., m_maxViewTime - m_playTimer.ElapsedTime())), 40.));
	s_QuadMesh->Draw();
}

void VideoSceneRenderer::reset()
{
	m_playTimer.Stop();
	m_playTimer.Start();
	m_CurrentIndex = 0;
}

std::string VideoSceneRenderer::genFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform float alpha;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "FragColour = alpha * texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

void VideoSceneRenderer::incrementFrame()
{
	m_CurrentIndex = (m_CurrentIndex + 1) % m_images->size();
}
