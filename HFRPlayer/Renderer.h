#pragma once
/* SIMPLE 2D IMAGE RENDERER (OPEN GL)*/

#include <string>
#include <vector>

#include "Model.h"
#include "GLTextureLoader.h"
#include "Stopwatch.h"

class Renderer
{
private:
	static GLFWwindow* s_GLWindow;

	//callbacks
	static void Error_callback(int error, const char* description);
	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosChange_callback(GLFWwindow* window, double mouseX, double mouseY);
	static void MouseButtonPress_callback(GLFWwindow* window, int button, int press_release, int mods);

	static Stopwatch s_FpsCountTimer;

	static Stopwatch s_FpsLimiter;
	static uint32_t s_framePhase;
	static float s_TargetFrameTime;
	static uint32_t s_frameRepeatCount;

	static bool s_Running;
	static std::vector<GLuint> s_Pictures;
	static unsigned s_CurrentIndex;
	static unsigned s_FrameCounter;

private:

	static std::string GenVertexShader();
	static std::string GenFragmentShader();


public:
	static void SetPictures(GLuint* IDs, int count);
	static void SetPictures(std::vector<GLuint>& IDs);
	static void SetFPS(float fps);
	static bool Init(int w, int h, std::string title, bool fullScreen = 0);
	static void Run();
	static void Cleanup();

	//  Class intended as static
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

