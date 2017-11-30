#pragma once
/* SIMPLE 2D IMAGE RENDERER (OPEN GL)*/

#include <string>
#include <vector>

#include "Mesh.h"

#include "ShaderProgram.h"
#include "Stopwatch.h"
#include "FastImgLoader.h"

enum class RendererState {Playing = 0, Loading = 1, WaitingForUser = 2 };
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
	static Mesh* s_QuadMesh;
	static ShaderProgram* s_PicturesShader;

	static Stopwatch s_FpsLimiter;
	static uint32_t s_framePhase;
	static uint64_t s_TargetFrameTime;
	static uint32_t s_frameRepeatCount;

	static RendererState s_RendererState;
	static std::vector<GLuint> s_Pictures;
	static unsigned s_CurrentIndex;
	static unsigned s_FrameCounter;

	static GLuint s_LoadingScrTexID;
	static GLuint s_ReadyScrTexID;

	static std::string s_Name;


private:

	static std::string GenVertexShader();
	static std::string GenFragmentShader();
	static void LoadInterfaceTextures();
	static void DisplayLoadingScreen();
	static bool LoadNewSet(const std::string& name);

public:
	static void SetPictures(GLuint* IDs, int count);
	static void SetPictures(std::vector<GLuint>& IDs);
	static void LoadTextures(const std::string& name);
	static void SetFPS(float fps);
	static bool Init(int w, int h, std::string title, bool fullScreen = 0);
	static void Run();

	//  Class intended as static
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

