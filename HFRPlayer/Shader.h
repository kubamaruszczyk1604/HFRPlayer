#pragma once
#include <string>


#define GLEW_STATIC 
//Include GLEW  
#include <GL/glew.h>  
//Include GLFW  
#include <GLFW/glfw3.h>  

class Shader
{
public:
	Shader(GLenum type);
	~Shader();

	bool LoadFromFile(const std::string& path);
	bool Compile(std::string& log);
	GLuint GetID();

private:
	GLuint m_Id;

};

