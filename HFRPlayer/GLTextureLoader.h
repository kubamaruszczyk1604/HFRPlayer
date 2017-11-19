#pragma once
//Include GLEW  
#include <GL/glew.h>  

//Include GLFW  
#include <GLFW/glfw3.h>  

#include<string>
#include "FreeImage.h"
#pragma comment(lib, "FreeImage.lib")


class GLTextureLoader
{
public:
	GLTextureLoader() = default;
	~GLTextureLoader();

	GLTextureLoader& operator=(const GLTextureLoader&) = delete;
	GLTextureLoader(const GLTextureLoader&) = delete;

	static GLuint LoadTexture(const std::string& path);
};

