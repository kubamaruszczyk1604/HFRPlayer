#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Stopwatch.h"

#include <string>
#include <glew.h>

class ISceneRenderer
{
public:
	// virtual dtor
	virtual ~ISceneRenderer() { delete m_shader; }

	virtual void render() = 0;
	virtual void reset() = 0;


	static void initialise();
	static void cleanup();

protected:
	static Mesh* s_QuadMesh;
	static Stopwatch s_GlobalClock;
	static std::string genDefaultVertexShader();

protected:
	ShaderProgram* m_shader;
	virtual std::string genFragShader() = 0;
	void initialiseShader();
};