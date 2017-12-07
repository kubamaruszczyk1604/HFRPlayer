#pragma once
#include "ISceneRenderer.h"

class NoFilesSceneRenderer :
	public ISceneRenderer
{
public:
	NoFilesSceneRenderer();
	~NoFilesSceneRenderer();

	// Inherited via ISceneRenderer
	virtual void render() override;
	virtual void reset() override;

protected:
	// Inherited via ISceneRenderer
	virtual std::string genFragShader() override;

private:
	GLuint m_NoFilesScrTexID;
};

