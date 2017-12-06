#include "GLTextureLoader.h"

#include <iostream>


GLuint GLTextureLoader::LoadTexture(const std::string & path)
{
	FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(path.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(texFormat, path.c_str());
	if (!image) return -1;

	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo24Bits(image);
	FreeImage_Unload(temp);


	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);
	char* pixeles = (char*)FreeImage_GetBits(image);
	

	//Now generate the OpenGL texture object 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)pixeles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	FreeImage_Unload(image);

	//glfwPollEvents();
	return textureID;
}

FIBITMAP* GLTextureLoader::LoadImageRAM(const std::string& path)
{
	FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(path.c_str(), 0);
	FIBITMAP* image = FreeImage_Load(texFormat, path.c_str());
	if (!image) return nullptr;

	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo24Bits(image);
	FreeImage_Unload(temp);	

	return image;
}

GLuint GLTextureLoader::PushToGPU(FIBITMAP* image)
{
	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);
	char* pixeles = (char*)FreeImage_GetBits(image);

	//Now generate the OpenGL texture object 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)pixeles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glfwPollEvents();
	return textureID;
}

void GLTextureLoader::FreeImageMemory(FIBITMAP*& image)
{
	FreeImage_Unload(image);
	image = nullptr;
}

