#pragma once
#include "ISceneRenderer.h"

#include <vector>

class IVideoSceneRenderer :
	public ISceneRenderer
{
public:
	// create a video scene renderer
	IVideoSceneRenderer(std::vector<GLuint>* images) : ISceneRenderer(), m_images(images), m_CurrentIndex(0) { };

	// go to the next frame
	virtual void incrementFrame() = 0;

	// maximum time before video is dimmed (after reset is called)
	void setMaximumViewTime(double t) { m_maxViewTime = t; }

protected:
	Stopwatch m_playTimer;
	std::vector<GLuint>* m_images;
	unsigned m_CurrentIndex;
	double m_maxViewTime;
};

