#include "PrototypeVideoRenderer.h"

#include <sstream>
#include <iostream>

#define FOUR_SPLIT 1
#define TWO_SPLIT 2

#define SPLIT_TYPE TWO_SPLIT


PrototypeVideoRenderer::PrototypeVideoRenderer(std::vector<GLuint>* images) :
	IVideoSceneRenderer(images)
{
	initialiseShader();
	GLuint samplerID = glGetUniformLocation(m_shader->GetID(), "SCT_TEXTURE2D_1");
	m_shader->SetAsCurrent();
	glUniform1i(samplerID, 1);
}

PrototypeVideoRenderer::~PrototypeVideoRenderer()
{
}

void PrototypeVideoRenderer::render()
{
	m_shader->SetAsCurrent();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_halfCurrentIndex]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_CurrentIndex]);
	
	s_QuadMesh->Draw();
}

void PrototypeVideoRenderer::reset()
{
	m_playTimer.Stop();
	m_playTimer.Start();
	m_CurrentIndex = 0;
	m_halfCurrentIndex = 0;
}

std::string PrototypeVideoRenderer::genFragShader()
{
	std::ostringstream s;
	s << "#version 330\n\n"; 
	s << "in vec2 oUVs;\n\n";
	s << "out vec4 FragColour;\n\n";
	s << "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	s << "uniform sampler2D  SCT_TEXTURE2D_1;\n\n";
	s << "void main()\n{\n";
	s << "float right = (sign(oUVs.x - 0.5) + 1) * 0.5;\n";

#if SPLIT_TYPE == FOUR_SPLIT
	s << "vec4 tex0 =  texture2D(SCT_TEXTURE2D_0,oUVs*2);\n";
	s << "vec4 tex1 =  texture2D(SCT_TEXTURE2D_1,oUVs*2);\n";
#else
	s << "vec4 tex0 =  texture2D(SCT_TEXTURE2D_0,vec2(oUVs.x + 0.25, oUVs.y));\n";
	s << "vec4 tex1 =  texture2D(SCT_TEXTURE2D_1,vec2(oUVs.x - 0.25, oUVs.y));\n";
#endif
	s << "FragColour = mix(tex0, tex1, right); \n}\n";
	return s.str();
}

void PrototypeVideoRenderer::incrementFrame()
{
	if (m_halfCurrentIndex != m_CurrentIndex)
	{
		m_CurrentIndex = (m_CurrentIndex + 1) % m_images->size();
		m_halfCurrentIndex = m_CurrentIndex;
	}
	else
	{
		m_CurrentIndex = (m_CurrentIndex + 1) % m_images->size();
	}
	
}
