#include "GLTextureLoader.h"

#include <iostream>


GLuint GLTextureLoader::LoadTexture(const std::string & path)
{
	FREE_IMAGE_FORMAT texFormat = FreeImage_GetFileType(path.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(texFormat, path.c_str());
	if (!imagen) return -1;

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo24Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	//Now generate the OpenGL texture object 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)pixeles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	FreeImage_Unload(imagen);
	glfwPollEvents();
	return textureID;
}

