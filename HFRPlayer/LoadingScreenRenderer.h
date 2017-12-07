#pragma once
#include "ISceneRenderer.h"

#include <vector>

#define SHOW_LOADING_BAR 

class LoadingScreenRenderer :
	public ISceneRenderer
{
public:
	LoadingScreenRenderer();
	~LoadingScreenRenderer();

	bool LoadSet(const std::string & name, std::vector<GLuint>* textureIds, int offset);

	// Inherited via ISceneRenderer
	virtual void render() override;
	virtual void reset() override;

	// only call from main thread
	void updateProgress(int abs);

protected:

	// Inherited via ISceneRenderer
	virtual std::string genFragShader() override;

private:
	int m_progress;
	GLuint m_LoadingScrTexID;	
};

