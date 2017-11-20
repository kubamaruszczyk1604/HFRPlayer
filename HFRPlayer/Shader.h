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
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	~Shader();

	bool LoadFromFile(const std::string& path);
	void SetShaderString(const std::string& content);
	bool Compile(std::string& log);
	GLuint GetID();

private:
	GLuint m_Id;

};

