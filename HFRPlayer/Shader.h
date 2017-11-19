#pragma once
#include <string>
#include <GL/glew.h>  
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

