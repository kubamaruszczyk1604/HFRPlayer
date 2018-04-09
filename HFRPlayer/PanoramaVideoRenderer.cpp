#include "PanoramaVideoRenderer.h"

#include <sstream>
#include <iostream>

#define SUB_FACTOR 3


PanoramaVideoRenderer::PanoramaVideoRenderer(std::vector<GLuint>* images) :
	IVideoSceneRenderer(images),
	m_incrementPhase(0),
	m_fullTime(0.),
	m_halfTime(0.),
	m_imageWidth(1),
	m_imageHeight(1)
{
	initialiseShader();	

	m_offsetFullLoc = glGetUniformLocation(m_shader->GetID(), "imgFullOffset");
	m_offsetHalfLoc = glGetUniformLocation(m_shader->GetID(), "imgHalfOffset");

	m_watch.Start();
}

PanoramaVideoRenderer::~PanoramaVideoRenderer()
{
}

void PanoramaVideoRenderer::render()
{
	m_shader->SetAsCurrent();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (*m_images)[m_CurrentIndex]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	auto offset = getOffsetForTime(m_fullTime);
	glUniform2f(m_offsetFullLoc, offset[0], offset[1]);
	offset = getOffsetForTime(m_halfTime);
	glUniform2f(m_offsetHalfLoc, offset[0], offset[1]);

	s_QuadMesh->Draw();
}

void PanoramaVideoRenderer::reset()
{
	m_incrementPhase = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_imageWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_imageHeight);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// fit image on half a screen (i.e. on 1440 pixels tall) maintaining aspect ratio
	auto scaleLoc = glGetUniformLocation(m_shader->GetID(), "imgScale");
	glUniform2f(scaleLoc, m_imageHeight * 1.f / m_imageWidth , 1);

	m_watch.Stop();
	m_watch.Start();
}

std::string PanoramaVideoRenderer::genFragShader()
{
	std::ostringstream s;
	s << "#version 330\n\n";
	s << "in vec2 oUVs;\n\n";
	s << "out vec4 FragColour;\n\n";
	s << "uniform vec2 imgScale;";
	s << "uniform vec2 imgHalfOffset;";
	s << "uniform vec2 imgFullOffset;";
	s << "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	s << "void main()\n{\n";
	
	/*s << "float halfResPlace = 1 - (sign(oUVs.y - 0.5) + 1) * 0.5;\n";
	s << "vec2 rightUv = vec2(oUVs.x, oUVs.y * 2);\n";
	s << "vec2 leftUv = vec2(oUVs.x, oUVs.y * 2);\n";*/
	
	
	s << "float halfResPlace = (sign(oUVs.x - 0.5) + 1) * 0.5;\n";
	s << "vec2 rightUv = vec2((oUVs.x - 0.5) * 2, oUVs.y);\n";
	s << "vec2 leftUv = vec2(oUVs.x * 2, oUVs.y);\n";
	
	s << "vec4 tex0 =  texture2D(SCT_TEXTURE2D_0, leftUv * vec2(imgScale.x, imgScale.y) + imgFullOffset);\n";
	s << "vec4 tex1 =  texture2D(SCT_TEXTURE2D_0, rightUv * vec2(imgScale.x , imgScale.y) + imgHalfOffset);\n";
	s << "FragColour = mix(tex0, tex1, halfResPlace); \n}\n";
	return s.str();
}

glm::vec2 PanoramaVideoRenderer::getOffsetForTime(double t)
{
	return glm::vec2(0.37 + sin(t * 1.6)  * 0.07,  /*(m_imageHeight - 1440) / 2880.f*/ 0.f);
}

void PanoramaVideoRenderer::incrementFrame()
{
	m_fullTime += m_watch.ElapsedTime();
	m_watch.Stop();
	m_watch.Start();
	m_incrementPhase++;
	if (m_incrementPhase == SUB_FACTOR)
	{
		m_incrementPhase = 0;
		m_halfTime = m_fullTime;
	}
}
