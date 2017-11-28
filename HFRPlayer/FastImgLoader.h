#pragma once
#include<vector>
#include<queue>
#include<mutex>
#include <sys/stat.h>

#include "GLTextureLoader.h"

class FastImgLoader
{
private:
	static const int MAX_THREAD_COUNT;
	static void LoadSequence(const std::string& formant, int startAtIndex);
	static int s_RunningThreads;
	static std::vector<std::queue<FIBITMAP*>> s_Queues;
	static std::vector<std::queue<int>> s_TestInts;
	static std::mutex s_CounterMutex;
	static std::mutex s_ConsoleMutex;

public:
	FastImgLoader() = delete;
	FastImgLoader(const FastImgLoader&) = delete;
	FastImgLoader& operator=(const FastImgLoader&) = delete;
	~FastImgLoader();

	static bool LoadImages(const std::string& formant, std::vector<GLuint>& output);
	static bool LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures);
	static bool FileExists(const std::string& name);

};

