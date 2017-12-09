#include "PrototypeVideoRenderer.h"

#include <sstream>
#include <iostream>

#define FOUR_SPLIT 1
#define TWO_SPLIT 2
#define TWO_SPLIT_NARROW 3
#define TWO_SPLIT_VERTICAL 4

#define SPLIT_TYPE TWO_SPLIT_VERTICAL

#define SWAP_SIDES 0
#define VIDEO_MERGE 0


PrototypeVideoRenderer::PrototypeVideoRenderer(std::vector<GLuint>* images) :
	IVideoSceneRenderer(images)
{
	initialiseShader();
	GLuint samplerID = glGetUniformLocation(m_shader->GetID(), "SCT_TEXTURE2D_1");
	m_shader->SetAsCurrent();
	glUniform1i(samplerID, 1);
	samplerID = glGetUniformLocation(m_shader->GetID(), "SCT_TEXTURE2D_2");
	glUniform1i(samplerID, 2);
}

PrototypeVideoRenderer::~PrototypeVideoRenderer()
{
}

void PrototypeVideoRenderer::render()
{
	m_shader->SetAsCurrent();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_halfCurrentIndex]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_halfPrevIndex]);

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
	m_halfPrevIndex = m_images->size() - 1;
}

std::string PrototypeVideoRenderer::genFragShader()
{
	std::ostringstream s;
	s << "#version 330\n\n"; 
	s << "in vec2 oUVs;\n\n";
	s << "out vec4 FragColour;\n\n";
	s << "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	s << "uniform sampler2D  SCT_TEXTURE2D_1;\n\n";
	s << "uniform sampler2D  SCT_TEXTURE2D_2;\n\n";
	s << "void main()\n{\n";
	s << "float halfResPlace = (sign(oUVs.x - 0.5) + 1) * 0.5;\n";
#if SPLIT_TYPE == FOUR_SPLIT
	s << "vec2 rightUv = vec2(oUVs.x*2, 1 - abs(oUVs.y*2 - 1));\n";
	s << "vec2 leftUv = vec2(oUVs.x*2, 1 - abs(oUVs.y*2 - 1));\n";
#elif SPLIT_TYPE == TWO_SPLIT
	s << "vec2 rightUv = vec2(oUVs.x - 0.25, oUVs.y);\n";
	s << "vec2 leftUv = vec2(oUVs.x + 0.25, oUVs.y);\n";
#elif SPLIT_TYPE == TWO_SPLIT_NARROW
	s << "vec2 rightUv = vec2(oUVs.x * 2, oUVs.y);\n";
	s << "vec2 leftUv = vec2(oUVs.x  * 2, oUVs.y);\n";
#elif SPLIT_TYPE == TWO_SPLIT_VERTICAL
	s << "vec2 rightUv = vec2(oUVs.x, oUVs.y * 2);\n";
	s << "vec2 leftUv = vec2(oUVs.x, oUVs.y * 2);\n";
	s << "halfResPlace = 1 - (sign(oUVs.y - 0.5) + 1) * 0.5;\n";
#else
#warning("No Prototyping technique")
#endif

#if SWAP_SIDES
	s << "halfResPlace = 1 - halfResPlace;\n";
	s << "vec2 temp = rightUv; rightUv = leftUv; leftUv = temp;\n";
#endif


	s << "vec4 tex1 =  texture2D(SCT_TEXTURE2D_1, rightUv);\n";
#if VIDEO_MERGE
	s << "tex1 = tex1 * 0.5 + 0.5 * texture2D(SCT_TEXTURE2D_2, rightUv);\n";
#endif

	s << "vec4 tex0 =  texture2D(SCT_TEXTURE2D_0, leftUv);\n";
	s << "FragColour = mix(tex0, tex1, halfResPlace); \n}\n";
	return s.str();
}

void PrototypeVideoRenderer::incrementFrame()
{
	if (m_halfCurrentIndex != m_CurrentIndex)
	{
		m_halfPrevIndex = m_CurrentIndex;
		m_CurrentIndex = (m_CurrentIndex + 1) % m_images->size();
		m_halfCurrentIndex = m_CurrentIndex;
	}
	else
	{
		m_CurrentIndex = (m_CurrentIndex + 1) % m_images->size();
	}
	
}
