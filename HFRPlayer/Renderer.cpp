#include "Renderer.h"

GLFWwindow* Renderer::s_GLWindow= nullptr;

void Renderer::Error_callback(int error, const char * description)
{
}

void Renderer::Key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
}

void Renderer::MousePosChange_callback(GLFWwindow * window, double mouseX, double mouseY)
{
}

void Renderer::MouseButtonPress_callback(GLFWwindow * window, int button, int press_release, int mods)
{
}

bool Renderer::Start(int w, int h, std::string title, bool fullScreen)
{
	//Set the error callback  
	//glfwSetErrorCallback(Error_callback);
	//glewExperimental = GL_TRUE;
	if (!glfwInit())
	{
		return false;
	}
	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
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

	glEnable(GL_DEPTH_TEST);

	//render loop
	float time = 0;
	bool was = true;
	do
	{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//time += 0.01f;
		//glClearColor(abs(sin(time)), abs(cos(time)), 0, 1);

		if (was)
		{
			glClearColor(1, 0, 0, 1);
			was = false;
		}
		else
		{
			glClearColor(0, 1, 0, 1);
			was = true;
		}

		glfwSwapBuffers(s_GLWindow);

		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(s_GLWindow));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(s_GLWindow);
	//Finalize and clean up GLFW  
	glfwTerminate();

}




Renderer::~Renderer()
{
}
