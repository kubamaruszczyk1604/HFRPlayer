#include "Renderer.h"
#include "FastImgLoader.h"
#include <iostream>


GLFWwindow* Renderer::s_GLWindow{ nullptr };
float Renderer::s_GlobalTime{ 0 };
bool Renderer::s_GlobalTimeThreadRunningFlag{ true };

Stopwatch Renderer::s_FpsCountTimer;
uint64_t Renderer::s_TargetFrameTime { 1000000000L /120};
uint32_t Renderer::s_frameRepeatCount{ 1 };
uint32_t Renderer::s_framePhase{ 1 };
RendererState Renderer::s_RendererState{ RendererState::Playing };
std::vector<GLuint> Renderer::s_Pictures;
unsigned Renderer::s_CurrentIndex{ 0 };
unsigned Renderer::s_FrameCounter{ 0 };
Networking::ExperimentSocketStream* Renderer::s_activeConnection = nullptr;


Mesh*  Renderer::s_QuadMesh;
ShaderProgram* Renderer::s_PicturesShader;
ShaderProgram* Renderer::s_LoadingShader;

GLuint Renderer::s_LoadingScrTexID{ 0 };
GLuint Renderer::s_ReadyScrTexID{ 0 };

std::string Renderer::s_Name{ "" };

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
			if (s_RendererState == RendererState::WaitingForUser) s_RendererState = RendererState::Playing;
		}
		else if (s_activeConnection)
		{ 
			// should we remap the weird glfw to standard Win VK?
			s_activeConnection->write((char)key);
		}
			//glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Renderer::MousePosChange_callback(GLFWwindow * window, double mouseX, double mouseY)
{
}

void Renderer::MouseButtonPress_callback(GLFWwindow * window, int button, int press_release, int mods)
{
}


void Renderer::TimeIncrementFunction()
{
	Stopwatch s_TimeDeltaSW;
	s_TimeDeltaSW.Start();
	while (s_GlobalTimeThreadRunningFlag)
	{
		s_GlobalTime = s_TimeDeltaSW.ElapsedTime();	
	}
	s_TimeDeltaSW.Stop();
}

std::string Renderer::GenVertexShader()
{
	std::string vertexShader = "#version 330\n\n";
	vertexShader += "layout(location = 0) in vec3 vertex_position;\n";
	vertexShader += "layout(location = 2) in vec2 uvs;\n\n";
	vertexShader += "out vec2 oUVs;\n\n";
	vertexShader += "void main()\n{\n";
	vertexShader += "  oUVs = uvs;\n";
	vertexShader += "   gl_Position =  vec4(vertex_position,1.0);\n}";
	return vertexShader;
}

std::string Renderer::GenFragmentShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform float time;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "FragColour = texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

std::string Renderer::GenLoadingFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform float time;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "FragColour = texture2D(SCT_TEXTURE2D_0,oUVs)*(0.5+abs(sin(time)));\n}\n";
	return fragmentShader;
}

void Renderer::LoadInterfaceTextures()
{
	s_LoadingScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/loading_screen.png");
	s_ReadyScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/player_ready.png");
}

void Renderer::DisplayLoadingScreen()
{
	glBindTexture(GL_TEXTURE_2D, s_LoadingScrTexID);
	s_QuadMesh->Draw();
	glfwSwapBuffers(s_GLWindow);
}

void Renderer::DisplayWaitForUserScreen()
{
	glBindTexture(GL_TEXTURE_2D, s_ReadyScrTexID);
	s_QuadMesh->Draw();
	glfwSwapBuffers(s_GLWindow);
}

bool Renderer::LoadSet(const std::string & name)
{
	DisplayLoadingScreen();
	std::cout << "Deleting textures..." << std::endl;
	// free all textures first
	glDeleteTextures(s_Pictures.size(), &s_Pictures[0]);
	FastImgLoader::LoadImages(name, s_Pictures);
	return 0;
}


void Renderer::LoadTextures(const std::string & name, Networking::ExperimentSocketStream* connection)
{
	if (s_RendererState == RendererState::Loading) return;
	s_Name = name;
	s_RendererState = RendererState::Loading;

	if (connection)
	{
		s_activeConnection = connection;
	}
}

void Renderer::SetFPS(float fps)
{
	// round down to the nearest integer
	int aFps = (int)fps;

	// assume max refresh rate of 165. TODO: should this come from a config as well?
	s_frameRepeatCount = 165 / aFps;
	aFps = aFps * s_frameRepeatCount;
	s_TargetFrameTime = 1000000000L / aFps;
}

bool Renderer::Init(int w, int h, std::string title, bool fullScreen)
{

	glfwSetErrorCallback(Error_callback);
    glewExperimental = GL_TRUE;

	if (!glfwInit()){ return false;}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	if(fullScreen)
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

	s_PicturesShader = new ShaderProgram(GenVertexShader(), GenFragmentShader(), ShaderStringType::Content);
	s_LoadingShader = new ShaderProgram(GenVertexShader(), GenLoadingFragShader(), ShaderStringType::Content);
	Vertex vertices[] =
	{
		//GL UVs ORDERING
		Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)),
		Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(1,0)),
		Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1))
	};

	s_QuadMesh = new Mesh();
	s_QuadMesh->Create(vertices, 4);

	int indices[] = { 0,1,2,0,2,3 };
	s_QuadMesh->CreateIndexBuffer(indices, 6);

	GLuint samplerID = glGetUniformLocation(s_PicturesShader->GetID(), "SCT_TEXTURE2D_0"); 
	glUniform1i(samplerID, 0);
	glActiveTexture(GL_TEXTURE0);
	s_PicturesShader->SetAsCurrent();
	LoadInterfaceTextures();

	
}

void Renderer::Run()
{
	std::thread globalTimeThread(TimeIncrementFunction);

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
			if (s_framePhase >= s_frameRepeatCount)
			{
				s_CurrentIndex++;
				if (s_CurrentIndex >= sizeCached) s_CurrentIndex = 0;
				s_framePhase = 1;
			}
			else
			{
				s_framePhase++;
			}
			glBindTexture(GL_TEXTURE_2D, s_Pictures[s_CurrentIndex]);
			s_QuadMesh->Draw();

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

		else if(s_RendererState == RendererState::Loading)
		{
			
			s_LoadingShader->SetAsCurrent();
			GLuint loc = glGetUniformLocation(s_LoadingShader->GetID(), "time");
			glUniform1f(loc, 1.0);
			LoadSet(s_Name);	

			// flush events that happened while loading
			glfwPollEvents();

			s_RendererState = RendererState::WaitingForUser;
			
		}
		else if (s_RendererState == RendererState::WaitingForUser)
		{
			s_PicturesShader->SetAsCurrent();
			
			DisplayWaitForUserScreen();
			sizeCached = s_Pictures.size();
			s_CurrentIndex = 0;
			s_FpsCountTimer.Start();
			currTime = std::chrono::high_resolution_clock::time_point();
			glfwPollEvents();
			
		}


	} while (!glfwWindowShouldClose(s_GLWindow));

	s_GlobalTimeThreadRunningFlag = false;
	globalTimeThread.join();
	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();
}


Renderer::~Renderer()
{
	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();

	//clean up
	glDeleteTextures(1, &s_LoadingScrTexID);
	glDeleteTextures(1, &s_ReadyScrTexID);
	glDeleteTextures(s_Pictures.size(), &s_Pictures[0]);
	delete (s_PicturesShader);
	delete(s_QuadMesh);
}
