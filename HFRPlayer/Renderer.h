#pragma once
/* SIMPLE 2D IMAGE RENDERER (OPEN GL)*/

#include <string>
#include <vector>

#include "ExperimentSocketStream.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Stopwatch.h"
#include "FastImgLoader.h"
#define SHOW_LOADING_BAR
enum class RendererState { Playing = 0, Loading = 1, WaitingForUser = 2, FailedToLoad = 3 };
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
	static ShaderProgram* s_FadeShader;
	static ShaderProgram* s_LoadBarShader;

	static uint32_t s_framePhase;
	static uint64_t s_TargetFrameTime;
	static uint32_t s_frameRepeatCount;

	static RendererState s_RendererState;
	static std::vector<GLuint> s_Pictures;
	static unsigned s_CurrentIndex;
	static unsigned s_FrameCounter;

	static GLuint s_LoadingScrTexID;
	static GLuint s_ReadyScrTexID;
	static GLuint s_NoFilesScrTexID;

	static std::string s_Name;
	static int s_LoadOffset;

	static float s_GlobalTime;
	static bool s_GlobalTimeThreadRunningFlag;


	// play timer and disappearing video
	static Stopwatch s_playTimer;
	static double s_maxViewTime;
private:
	
	static void TimeIncrementFunction();
	static std::string GenVertexShader();
	static std::string GenFragmentShader();
	static std::string GenFadeFragShader();
	static std::string GenLoadBarFragShader();
	static void LoadInterfaceTextures();
	static void DisplayLoadingScreen(int progress);
	static void DisplayWaitForUserScreen();
	static void DisplayNoFilesFoundScreen();
	static bool LoadSet(const std::string& name); //Internal, non thread-safe

	static Networking::ExperimentSocketStream* s_activeConnection;

public:

	// Please use this method to load new set of textures. optional socket indicates the network connection which initiates the load
	static void LoadTextures(const std::string& name, int offset, Networking::ExperimentSocketStream* throughConnection = nullptr);
	static void SetFPS(float fps);
	static void SetMaximumViewTime(double t) { s_maxViewTime = t;}
	static bool Init(int w, int h, std::string title, bool fullScreen = 0);
	static void Run();



	//  Class intended as static
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

