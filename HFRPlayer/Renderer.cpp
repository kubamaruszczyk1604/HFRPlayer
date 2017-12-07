#include "Renderer.h"


#include "FastImgLoader.h"
#include "ISceneRenderer.h"
#include "NoFilesSceneRenderer.h"
#include "VideoSceneRenderer.h"
#include "WaitForInputSceneRenderer.h"
#include "PrototypeVideoRenderer.h"

#include <iostream>
#include <chrono>
#include <thread>


GLFWwindow* Renderer::s_GLWindow{ nullptr };

Stopwatch Renderer::s_FpsCountTimer;
uint64_t Renderer::s_TargetFrameTime { 1000000000L /120};
uint32_t Renderer::s_frameRepeatCount { 1 };
RendererState Renderer::s_RendererState{ RendererState::Playing };
unsigned Renderer::s_FrameCounter{ 0 };
std::vector<GLuint> Renderer::s_Pictures;
Networking::ExperimentSocketStream* Renderer::s_activeConnection = nullptr;

// Config. Maybe thgis should just use the Configuration encapsulation?
double Renderer::s_maxViewTime{ DBL_MAX };
std::string Renderer::s_Name{ "" };
int Renderer::s_LoadOffset{ 0 };

LoadingScreenRenderer*  Renderer::s_loadingScreenRenderer = nullptr;
ISceneRenderer*  Renderer::s_noFilesRenderer = nullptr;
IVideoSceneRenderer*  Renderer::s_VideoSceneRenderer = nullptr;
ISceneRenderer*  Renderer::s_waitForInputRenderer = nullptr;

void Renderer::Error_callback(int error, const char * description)
{
	fputs(description, stderr);
	_fgetchar();
}

void Renderer::Key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (action ==  GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE) 
		{ 
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_ENTER)
		{
			if (s_RendererState == RendererState::WaitingForUser)
			{
				s_RendererState = RendererState::Playing;
				s_FpsCountTimer.Start();
				s_VideoSceneRenderer->reset();
			}
		}
		else if (key == GLFW_KEY_BACKSPACE)
		{
			if (s_RendererState == RendererState::Playing)
			{
				s_VideoSceneRenderer->reset();
			}
		}
		else if (s_activeConnection)
		{
			// should we remap the weird glfw to standard Win VK?
			s_activeConnection->write((char)key);
		}
	}
}

void Renderer::MousePosChange_callback(GLFWwindow * window, double mouseX, double mouseY)
{
}

void Renderer::MouseButtonPress_callback(GLFWwindow * window, int button, int press_release, int mods)
{
}

void Renderer::LoadTextures(const std::string & name, int offset, Networking::ExperimentSocketStream* connection)
{
	if (s_RendererState == RendererState::Loading) return;
	s_Name = name;
	s_LoadOffset = offset;
	s_RendererState = RendererState::Loading;

	if (connection)
	{
		s_activeConnection = connection;
	}
}

void Renderer::SetFPS(float fps)
{
	std::cout << "setting fps " << fps << std::endl;

	// round down to the nearest integer
	int aFps = (int)fps;

	// assume max refresh rate of 165. TODO: should this come from a config as well?
	s_frameRepeatCount = 165 / aFps;
	aFps = aFps * s_frameRepeatCount;
	s_TargetFrameTime = 1000000000L / aFps;
}

bool Renderer::Init(int w, int h, std::string title, bool fullScreen)
{
	srand(time(NULL));

	glfwSetErrorCallback(Error_callback);
	glewExperimental = GL_TRUE;

	if (!glfwInit()) { return false; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (fullScreen)
		s_GLWindow = glfwCreateWindow(w, h, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else
		s_GLWindow = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	if (!s_GLWindow)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(s_GLWindow);
	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}
	glfwSetKeyCallback(s_GLWindow, Key_callback);

	ISceneRenderer::initialise();
	s_loadingScreenRenderer = new LoadingScreenRenderer();
	s_noFilesRenderer = new NoFilesSceneRenderer();
	s_waitForInputRenderer = new WaitForInputSceneRenderer();
#if PROTOTYPE_MODE
	s_VideoSceneRenderer = new PrototypeVideoRenderer(&s_Pictures);
#else
	s_VideoSceneRenderer = new VideoSceneRenderer(&s_Pictures);
#endif
}

void Renderer::Run()
{
	verifyNoError();

	unsigned sizeCached = s_Pictures.size();
	s_FpsCountTimer.Start();
	std::chrono::high_resolution_clock::time_point previousDispTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point currTime;
	// main loop
	do
	{
		//render loop
		if (s_RendererState == RendererState::Playing)
		{
			for (int i = 0; i < s_frameRepeatCount; ++i)
			{
				s_VideoSceneRenderer->render();
				while (currTime = std::chrono::high_resolution_clock::now(), std::chrono::duration<uint64_t, std::nano>((currTime - previousDispTime).count()).count() < s_TargetFrameTime)
				{
				}
				previousDispTime = currTime;
				glfwSwapBuffers(s_GLWindow);

				s_FrameCounter++;
				if (s_FpsCountTimer.ElapsedTime() >= 1)
				{
					std::cout << "FPS: " << s_FrameCounter << std::endl;
					s_FpsCountTimer.Stop();
					s_FrameCounter = 0;
					s_FpsCountTimer.Start();
				}
				glfwPollEvents();
			}	
			s_VideoSceneRenderer->incrementFrame();
		}
		else if(s_RendererState == RendererState::Loading)
		{
			s_RendererState = s_loadingScreenRenderer->LoadSet(s_Name, &s_Pictures, s_LoadOffset) ? 
				RendererState::WaitingForUser : RendererState::FailedToLoad;
			glfwPollEvents();
			verifyNoError();
		}
		else if (s_RendererState == RendererState::WaitingForUser)
		{
			s_waitForInputRenderer->render();
			glfwSwapBuffers(s_GLWindow);
			glfwPollEvents();
		}
		else if (s_RendererState == RendererState::FailedToLoad)
		{
			s_noFilesRenderer->render();
			glfwSwapBuffers(s_GLWindow);
			glfwPollEvents();
		}
		verifyNoError();

	} while (!glfwWindowShouldClose(s_GLWindow));

	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();
}

void Renderer::RequestBufferSwap()
{
	glfwSwapBuffers(s_GLWindow);
}

Renderer::~Renderer()
{
	//clean up
	glDeleteTextures(s_Pictures.size(), &s_Pictures[0]);

	ISceneRenderer::cleanup();
	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();	
}
