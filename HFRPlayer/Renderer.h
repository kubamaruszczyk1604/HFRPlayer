#pragma once

#include "Model.h"
#include <string>
#include <vector>
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

	static Stopwatch s_Stopwatch;
	static bool s_Running;
	static std::vector<GLuint> s_Pictures;
	static unsigned s_CurrentIndex;
	static unsigned s_FrameCounter;

public:
	static void SetPictures(GLuint* IDs, int count);
	static void SetPictures(std::vector<GLuint>& IDs);
	static void SetFPS(float fps);
	static bool Init(int w, int h, std::string title, bool fullScreen = 0);
	static void Run();
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

