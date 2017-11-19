#include "Renderer.h"
#include "Model.h"


GLFWwindow* Renderer::s_GLWindow{ nullptr };

Stopwatch Renderer::s_Stopwatch;
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


void Renderer::SetPictures(GLuint * IDs, int count)
{
}

void Renderer::SetPictures(std::vector<GLuint>& IDs)
{
	s_Pictures = IDs;
}

void Renderer::SetFPS(float fps)
{
}

bool Renderer::Init(int w, int h, std::string title, bool fullScreen)
{
	//Set the error callback  
	glfwSetErrorCallback(Error_callback);
    glewExperimental = GL_TRUE;
	if (!glfwInit())
	{
		return false;
	}
	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window and create its OpenGL context  
	if(fullScreen)
	s_GLWindow = glfwCreateWindow(w, h, title.c_str(), glfwGetPrimaryMonitor(), NULL);
	else 
		s_GLWindow = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	//If the window couldn't be created  
	if (!s_GLWindow)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(s_GLWindow);

	//Initialize GLEW  
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
	ShaderProgram* sp = new ShaderProgram("C:/Zapas/glVert.txt", "c:/Zapas/glFrag.txt");

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
	s_Stopwatch.Start();
	//render loop
	bool was = true;
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);


		sp->SetAsCurrent();//Shader
		s_CurrentIndex++;
		if (s_CurrentIndex >= sizeCached) s_CurrentIndex = 0;
		glBindTexture(GL_TEXTURE_2D, s_Pictures[s_CurrentIndex]);
		
		mesh->Draw();
		glfwSwapBuffers(s_GLWindow);

		s_FrameCounter++;
		if (s_Stopwatch.ElapsedTime() >= 1)
		{
			std::cout << "FPS: " << s_FrameCounter << std::endl;
			s_Stopwatch.Stop();
			s_FrameCounter = 0;
			s_Stopwatch.Start();
		}
		glfwPollEvents();

	} while (!glfwWindowShouldClose(s_GLWindow) && s_Running);


	glfwDestroyWindow(s_GLWindow);
	glfwTerminate();

	//clean up
	delete (sp);
	delete(mesh);
}




Renderer::~Renderer()
{
}
