#include "Renderer.h"
#include "Model.h"


GLFWwindow* Renderer::s_GLWindow{ nullptr };

Stopwatch Renderer::s_FpsCountTimer;
Stopwatch Renderer::s_FpsLimiter;
uint64_t Renderer::s_TargetFrameTime { 1000000000L /120};
uint32_t Renderer::s_frameRepeatCount{ 1 };
uint32_t Renderer::s_framePhase{ 1 };
bool Renderer::s_Running{ true };
std::vector<GLuint> Renderer::s_Pictures;
unsigned Renderer::s_CurrentIndex{ 0 };
unsigned Renderer::s_FrameCounter{ 0 };


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
			glfwSetWindowShouldClose(window, GL_TRUE);
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
	vertexShader += "layout(location = 1) in vec3 vertex_normal;\n";
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
	fragmentShader += "void main()\n{\n";
	fragmentShader += "FragColour = texture2D(SCT_TEXTURE2D_0,oUVs);\n}\n";
	return fragmentShader;
}

void Renderer::SetPictures(GLuint * IDs, int count)
{
}

void Renderer::SetPictures(std::vector<GLuint>& IDs)
{
	s_Pictures = IDs;
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

	if (!glfwInit())
	{
		return false;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	//glfwWindowHint(GLFW_SAMPLES, 4); 
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

	//GLEW  stuff init
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}
	glfwSetKeyCallback(s_GLWindow, Key_callback);
		
}

void Renderer::Run()
{
	//glEnable(GL_DEPTH_TEST);
	//ShaderProgram* sp = new ShaderProgram("C:/Test/glVert.txt", "c:/Test/glFrag.txt",ShaderStringType::Path);
	ShaderProgram* sp = new ShaderProgram(GenVertexShader(), GenFragmentShader(), ShaderStringType::Content);
	Vertex vertices[] =
	{
		//Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)),
		//Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)),
		//Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(1,1)),
		//Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 0))

		//GL UVs ORDERING
		Vertex(glm::vec3(-1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)),
		Vertex(glm::vec3(-1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)),
		Vertex(glm::vec3(1, -1, 0), glm::vec3(0, 0, 0), glm::vec2(1,0)),
		Vertex(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1))
	};

	Mesh* mesh = new Mesh();
	mesh->Create(vertices, 4);

	int indices[] = { 0,1,2,0,2,3 };
	mesh->CreateIndexBuffer(indices, 6);

	//Model* model = new Model(sp, mesh);
	GLuint samplerID = glGetUniformLocation(sp->GetID(), "SCT_TEXTURE2D_0"); // can be done only onse in this case
	glUniform1i(samplerID, 0);//loc, value
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texID2);

	unsigned sizeCached = s_Pictures.size();

	s_FpsCountTimer.Start();
	s_FpsLimiter.Start();

	sp->SetAsCurrent();//Shader

	std::chrono::high_resolution_clock::time_point previousDispTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point currTime;

	//render loop
	do
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
		
		// draw
		glBindTexture(GL_TEXTURE_2D, s_Pictures[s_CurrentIndex]);
		mesh->Draw();

		while (currTime = std::chrono::high_resolution_clock::now(), std::chrono::duration<uint64_t, std::nano>((currTime - previousDispTime).count()).count() < s_TargetFrameTime)
		{
		}
		previousDispTime = currTime;

		glfwSwapBuffers(s_GLWindow);
		s_FpsLimiter.Stop();
		s_FpsLimiter.Start();
		

		s_FrameCounter++;
		if (s_FpsCountTimer.ElapsedTime() >= 1)
		{
			std::cout << "FPS: " << s_FrameCounter << std::endl;
			s_FpsCountTimer.Stop();
			s_FrameCounter = 0;
			s_FpsCountTimer.Start();
		}
		glfwPollEvents();

	} while (!glfwWindowShouldClose(s_GLWindow) && s_Running);


	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();

	//clean up
	delete (sp);
	delete(mesh);
}

void Renderer::Cleanup()
{
	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();
}


Renderer::~Renderer()
{
}
