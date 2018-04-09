#pragma once
#include "IVideoSceneRenderer.h"
#include "Stopwatch.h"


// rendering a moving panorama image full and half frame rate side-by-side 
class PanoramaVideoRenderer :
	public IVideoSceneRenderer
{
public:
	PanoramaVideoRenderer(std::vector<GLuint>* images);
	~PanoramaVideoRenderer();

	// Inherited via IVideoSceneRenderer
	virtual void render() override;
	virtual void reset() override;

	virtual void incrementFrame() override;

protected:
	virtual std::string genFragShader() override;

	// image dimensions
	int m_imageWidth;
	int m_imageHeight;

	// Shader locs
	GLuint m_offsetHalfLoc;
	GLuint m_offsetFullLoc;

	// times/offsets
	int m_incrementPhase;
	Stopwatch m_watch;
	double m_fullTime;
	double m_halfTime;
	glm::vec2 getOffsetForTime(double t);
};

