#pragma once
#include<vector>
#include<queue>
#include <sys/stat.h>

#include "GLTextureLoader.h"

class FastImgLoader
{
private:
	static const int MAX_THREAD_COUNT;

	static void LoadSequence(const std::string& formant, int startAtIndex, std::queue<FIBITMAP*>& outQueue);

public:
	FastImgLoader() = delete;
	FastImgLoader(const FastImgLoader&) = delete;
	FastImgLoader& operator=(const FastImgLoader&) = delete;
	~FastImgLoader();

	static bool LoadImages(const std::string& formant, std::vector<GLuint>& output);
	static bool LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures);
	static bool FileExists(const std::string& name);

};

