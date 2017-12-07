#include "Renderer.h"
#include "FastImgLoader.h"
#include <iostream>
#include <chrono>
#include <thread>

GLFWwindow* Renderer::s_GLWindow{ nullptr };
Stopwatch Renderer::s_GlobalClock;

double Renderer::s_maxViewTime{ DBL_MAX };
Stopwatch Renderer::s_FpsCountTimer;
Stopwatch Renderer::s_playTimer;
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
ShaderProgram* Renderer::s_FadeShader;
ShaderProgram* Renderer::s_LoadBarShader;

GLuint Renderer::s_LoadingScrTexID{ 0 };
GLuint Renderer::s_ReadyScrTexID{ 0 };
GLuint Renderer::s_NoFilesScrTexID{ 0 };

std::string Renderer::s_Name{ "" };
int Renderer::s_LoadOffset{ 0 };

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
				s_playTimer.Stop();
				s_playTimer.Start();
			}
		}
		else if (key == GLFW_KEY_BACKSPACE)
		{
			s_playTimer.Stop();
			if (s_RendererState == RendererState::Playing)
			{
				s_playTimer.Start();
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

std::string Renderer::GenVertexShader()
{
	std::string vertexShader = "#version 330\n\n";
	vertexShader += "layout(location = 0) in vec3 vertex_position;\n";
	vertexShader += "layout(location = 1) in vec2 uvs;\n\n";
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
	fragmentShader += "uniform float alpha;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "FragColour = alpha * texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

std::string Renderer::GenFadeFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform vec3 col;\n\n";
	fragmentShader += "uniform float time;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "vec4 sampled = texture2D(SCT_TEXTURE2D_0,oUVs); \n";
	fragmentShader += "if((sampled.x + sampled.y + sampled.z) < 2.6) \n";
	fragmentShader += "FragColour =  sampled - (0.5*abs(sin(time))*vec4(col,1)); \n";
	fragmentShader += "else FragColour =  sampled; \n}\n";
	return fragmentShader;
}

std::string Renderer::GenLoadBarFragShader()
{
	std::string fragmentShader = "#version 330\n\n";
	fragmentShader += "in vec2 oUVs;\n\n";
	fragmentShader += "out vec4 FragColour;\n\n";
	fragmentShader += "uniform sampler2D  SCT_TEXTURE2D_0;\n\n";
	fragmentShader += "uniform float progress;\n\n";
	fragmentShader += "void main()\n{\n";
	fragmentShader += "if((oUVs.y > 0.7)&&(oUVs.y < 0.8)&&(oUVs.x < progress)) FragColour = texture2D(SCT_TEXTURE2D_0,oUVs)*0.5 + vec4(0.5-0.5*oUVs.x,0,oUVs.x,0.5); else \n";
	fragmentShader += "FragColour = texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

void Renderer::LoadInterfaceTextures()
{
	s_LoadingScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/loading_screen.png");
	s_ReadyScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/player_ready.png");
	s_NoFilesScrTexID = GLTextureLoader::LoadTexture("InterfaceImages/no_img_found.png");
}
int lp = 0;
void Renderer::DisplayLoadingScreen(int progress)
{
#ifdef SHOW_LOADING_BAR
	if (lp  < progress)
#endif
	{
		s_LoadBarShader->SetAsCurrent();
		GLuint loc = glGetUniformLocation(s_LoadBarShader->GetID(), "progress");
		glUniform1f(loc, (float)lp/500.0f);
		glBindTexture(GL_TEXTURE_2D, s_LoadingScrTexID);
		s_QuadMesh->Draw();
		glfwSwapBuffers(s_GLWindow);
		lp = progress + (rand() % 30 + 60);
	}
}

void Renderer::DisplayWaitForUserScreen()
{

	s_FadeShader->SetAsCurrent();

	GLuint loct = glGetUniformLocation(s_FadeShader->GetID(), "time");
	glUniform1f(loct, s_GlobalClock.ElapsedTime() * 1.5);
	GLuint locc = glGetUniformLocation(s_FadeShader->GetID(), "col");
	const float col[] = { 1.0,1.0,1.0 };
	glUniform3f(locc, 0.41, 0.41, 0);
	glBindTexture(GL_TEXTURE_2D, s_ReadyScrTexID);
	s_QuadMesh->Draw();
	glfwSwapBuffers(s_GLWindow);
}

void Renderer::DisplayNoFilesFoundScreen()
{
	s_FadeShader->SetAsCurrent();

	GLuint loct = glGetUniformLocation(s_FadeShader->GetID(), "time");
	glUniform1f(loct, s_GlobalClock.ElapsedTime());
	GLuint locc = glGetUniformLocation(s_FadeShader->GetID(), "col");
	const float col[] = { 1.0,1.0,1.0 };
	glUniform3f(locc, 0.3, 0.3, 0);
	glBindTexture(GL_TEXTURE_2D, s_NoFilesScrTexID);
	s_QuadMesh->Draw();
	glfwSwapBuffers(s_GLWindow);
}

bool Renderer::LoadSet(const std::string & name)
{	
	std::cout << "Deleting textures..." << std::endl;
	// free all textures first
	glDeleteTextures(s_Pictures.size(), &s_Pictures[0]);
	s_Pictures.clear();
#ifdef SHOW_LOADING_BAR
	return FastImgLoader::LoadImages(name, s_Pictures, s_LoadOffset, DisplayLoadingScreen);
#else
	return FastImgLoader::LoadImages(name, s_Pictures, s_LoadOffset);
#endif
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
	s_FadeShader = new ShaderProgram(GenVertexShader(), GenFadeFragShader(), ShaderStringType::Content);
	s_LoadBarShader = new ShaderProgram(GenVertexShader(), GenLoadBarFragShader(), ShaderStringType::Content);

	Vertex vertices[] =
	{
		//GL UVs ORDERING
		Vertex(glm::vec3(-1, 1, 0), glm::vec2(0, 1)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec2(0, 0)),
		Vertex(glm::vec3(1, -1, 0), glm::vec2(1,0)),
		Vertex(glm::vec3(1, 1, 0), glm::vec2(1, 1))
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
	unsigned sizeCached = s_Pictures.size();
	s_FpsCountTimer.Start();
	s_GlobalClock.Start();
	std::chrono::high_resolution_clock::time_point previousDispTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point currTime;

	GLuint alphaLoc = glGetUniformLocation(s_PicturesShader->GetID(), "alpha");

	// main loop
	do
	{
		//render loop
		if (s_RendererState == RendererState::Playing)
		{
			s_PicturesShader->SetAsCurrent();
			if (s_framePhase >= s_frameRepeatCount)
			{
				s_CurrentIndex = (s_CurrentIndex + 1) % sizeCached;
				s_framePhase = 1;
			}
			else
			{
				s_framePhase++;
			}
			glBindTexture(GL_TEXTURE_2D, s_Pictures[s_CurrentIndex]);
			s_QuadMesh->Draw();
			glUniform1f(alphaLoc, std::pow(std::min(1., std::max(0., s_maxViewTime - s_playTimer.ElapsedTime())), 40.));

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
			DisplayLoadingScreen(0);
			Stopwatch sw;
			sw.Start();
			if(LoadSet(s_Name)) s_RendererState = RendererState::WaitingForUser;
			else s_RendererState = RendererState::FailedToLoad;
			sw.Stop();
			std::cout << "LOADING IMAGES TOOK: " << std::to_string(sw.ElapsedTime()) << std::endl;

			lp = 0;
			// flush events that happened while loading
			glfwPollEvents();
			
		}
		else if (s_RendererState == RendererState::WaitingForUser)
		{
			
			DisplayWaitForUserScreen();
			sizeCached = s_Pictures.size();
			s_CurrentIndex = 0;
			s_FpsCountTimer.Start();
			currTime = std::chrono::high_resolution_clock::time_point();
			glfwPollEvents();
			
		}
		else if (s_RendererState == RendererState::FailedToLoad)
		{
			DisplayNoFilesFoundScreen();
			sizeCached = s_Pictures.size();
			s_CurrentIndex = 0;
			s_FpsCountTimer.Start();
			currTime = std::chrono::high_resolution_clock::time_point();
			glfwPollEvents();

		}

	} while (!glfwWindowShouldClose(s_GLWindow));

	s_GlobalClock.Stop();
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
	delete (s_FadeShader);
	delete (s_LoadBarShader);
	delete(s_QuadMesh);
}
