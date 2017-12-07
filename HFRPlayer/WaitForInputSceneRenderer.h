#pragma once
#include "ISceneRenderer.h"

class WaitForInputSceneRenderer :
	public ISceneRenderer
{
public:
	WaitForInputSceneRenderer();
	~WaitForInputSceneRenderer();

	// Inherited via ISceneRenderer
	virtual void render() override;
	virtual void reset() override;

protected:
	// Inherited via ISceneRenderer
	virtual std::string genFragShader() override;

private:
	GLuint m_ReadyScrTexID;
};

