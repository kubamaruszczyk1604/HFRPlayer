#include "LoadingScreenRenderer.h"
#include <WinSock2.h> // included to avoid name aliasing

#include "GLTextureLoader.h"
#include "FastImgLoader.h"
#include "Renderer.h"

#include <iostream>



LoadingScreenRenderer::LoadingScreenRenderer() :
	m_progress(0)
{
	initialiseShader();
	m_LoadingScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/loading_screen.png");
}

LoadingScreenRenderer::~LoadingScreenRenderer()
{
	glDeleteTextures(1, &m_LoadingScrTexID);
}

void LoadingScreenRenderer::render()
{
#ifdef SHOW_LOADING_BAR
	m_shader->SetAsCurrent();
	GLuint loc = glGetUniformLocation(m_shader->GetID(), "progress");
	glUniform1f(loc, (float)m_progress / 500.0f);
	glBindTexture(GL_TEXTURE_2D, m_LoadingScrTexID);
	s_QuadMesh->Draw();
	m_progress = m_progress;
#endif
}

void LoadingScreenRenderer::reset()
{
	m_progress = 0;
}

void LoadingScreenRenderer::updateProgress(int abs)
{
	m_progress = abs;
	render();
	Renderer::RequestBufferSwap();
}

std::string LoadingScreenRenderer::genFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform float progress;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "if((oUVs.y > 0.7)&&(oUVs.y < 0.8)&&(oUVs.x < progress)) FragColour = texture2D(SCT_TEXTURE2D_0,oUVs)*0.5 + vec4(0.5-0.5*oUVs.x,0,oUVs.x,0.5); else \n";
	fragmentShader += "FragColour = texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

bool LoadingScreenRenderer::LoadSet(const std::string & name, std::vector<GLuint>* textureIds, int offset)
{
	m_progress = 0;

	Stopwatch sw;
	sw.Start();

	std::cout << "Deleting textures..." << std::endl;

	// free all textures first
	glDeleteTextures(textureIds->size(), &(*textureIds)[0]);
	textureIds->clear();


#ifdef SHOW_LOADING_BAR
	bool res = FastImgLoader::LoadImages(name, textureIds, offset, this);
#else
	bool res = FastImgLoader::LoadImages(name, textureIds, offset, nullptr);
#endif

	sw.Stop();
	std::cout << "LOADING IMAGES TOOK: " << std::to_string(sw.ElapsedTime()) << std::endl;

	// flush events that happened while loading
	glfwPollEvents();

	return res;
}