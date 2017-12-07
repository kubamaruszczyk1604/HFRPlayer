#pragma once
#include "IVideoSceneRenderer.h"

#include <vector>

class VideoSceneRenderer :
	public IVideoSceneRenderer
{
public:
	VideoSceneRenderer(std::vector<GLuint>* images);
	~VideoSceneRenderer();

	// Inherited via ISceneRenderer
	virtual void render() override;
	virtual void reset() override;

	// Inherited via IVideoSceneRenderer
	virtual void incrementFrame() override;

protected:
	// Inherited via ISceneRenderer
	virtual std::string genFragShader() override;
	
	GLuint m_alphaLoc;
};

