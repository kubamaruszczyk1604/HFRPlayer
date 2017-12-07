#pragma once
#include "IVideoSceneRenderer.h"


// rendering full and half frame rate side-by-side 
class PrototypeVideoRenderer :
	public IVideoSceneRenderer
{
public:
	PrototypeVideoRenderer(std::vector<GLuint>* images);
	~PrototypeVideoRenderer();

	// Inherited via IVideoSceneRenderer
	virtual void render() override;
	virtual void reset() override;
	
	virtual void incrementFrame() override;

protected:
	virtual std::string genFragShader() override;
	int m_halfCurrentIndex;
};

