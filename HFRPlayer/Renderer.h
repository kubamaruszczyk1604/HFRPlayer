#pragma once

#include "Model.h"
#include <string>
#include <vector>

class Renderer
{
private:
	static GLFWwindow* s_GLWindow;

	//callbacks
	static void Error_callback(int error, const char* description);
	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosChange_callback(GLFWwindow* window, double mouseX, double mouseY);
	static void MouseButtonPress_callback(GLFWwindow* window, int button, int press_release, int mods);

	static bool s_Running;

	static std::vector<Model*> s_Models;

public:
	static void AddModel(Model* model);
	static void SetTextures(GLuint* IDs, int count);
	static void SetFPS(float fps);
	static bool Start(int w, int h, std::string title, bool fullScreen = 0);
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer();
};

