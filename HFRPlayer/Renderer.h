#pragma once

#include "Model.h"
#include <string>
#include <vector>

class Renderer
{
private:
	//Window object  
	static GLFWwindow* s_GLWindow;

	//callbacks
	//Define an error callback  
	static void Error_callback(int error, const char* description);
	//Define the key input callback  
	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//Define mouse pos change callback
	static void MousePosChange_callback(GLFWwindow* window, double mouseX, double mouseY);
	//Define mouse button press callback
	static void MouseButtonPress_callback(GLFWwindow* window, int button, int press_release, int mods);

	static bool s_Running;

	static std::vector<Model*> s_Models;

public:
	static void AddModel(Model* model);

	static bool Start(int w, int h, std::string title, bool fullScreen = 0);
	Renderer() = delete;
	~Renderer();
};

