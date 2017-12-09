#pragma once
/* SIMPLE 2D IMAGE RENDERER (OPEN GL)*/

#include <string>
#include <vector>

#include "ExperimentSocketStream.h"

#include "Mesh.h"

#include "Stopwatch.h"
#include "FastImgLoader.h"


#include "ISceneRenderer.h"
#include "IVideoSceneRenderer.h"
#include "LoadingScreenRenderer.h"

#include <iostream>

#define PROTOTYPE_MODE 1

enum class RendererState { Playing = 0, Loading = 1, WaitingForUser = 2, FailedToLoad = 3 };
class Renderer
{
private:
	// window
	static GLFWwindow* s_GLWindow;
	
	//callbacks
	static void Error_callback(int error, const char* description);
	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosChange_callback(GLFWwindow* window, double mouseX, double mouseY);
	static void MouseButtonPress_callback(GLFWwindow* window, int button, int press_release, int mods);

	// FPS counter
	static unsigned s_FrameCounter;
	static Stopwatch s_FpsCountTimer;

	// Gsync
	static uint32_t s_frameRepeatCount;
	static uint64_t s_TargetFrameTime;

	// state machine
	static RendererState s_RendererState;

	// video
	static std::vector<GLuint> s_Pictures;

	// config
	static std::string s_Name;
	static int s_LoadOffset;
	static double s_maxViewTime;
private:

	// sub renderers
	static LoadingScreenRenderer* s_loadingScreenRenderer;
	static ISceneRenderer* s_noFilesRenderer;
	static IVideoSceneRenderer* s_VideoSceneRenderer;
	static ISceneRenderer* s_waitForInputRenderer;

	// active network
	static Networking::ExperimentSocketStream* s_activeConnection;

public:

	// Please use this method to load new set of textures. optional socket indicates the network connection which initiates the load
	static void LoadTextures(const std::string& name, int offset, Networking::ExperimentSocketStream* throughConnection = nullptr);
	static void SetFPS(float fps);
	static void SetMaximumViewTime(double t) { s_maxViewTime = t; s_VideoSceneRenderer->setMaximumViewTime(s_maxViewTime); }
	static bool Init(int w, int h, std::string title, bool fullScreen = 0);
	static void Run();

	// request a buffer swap now
	static void RequestBufferSwap();

	static inline void verifyNoError()
	{
#ifdef _DEBUG
		if (GLenum error = glGetError())
		{
			std::cout << "OpenGL error: " << error << " - " << glewGetErrorString(error);
			throw ("OpenGL is in an invalid state");
		}
#endif
	}

	//  Class intended as static
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

