#pragma once
#include<vector>
#include<queue>
#include<mutex>
#include <sys/stat.h>

#include "GLTextureLoader.h"
#include "SafeQueue.h"
#include "LoadingScreenRenderer.h"

//#define HFR_ORDERING_TEST 1

class FastImgLoader
{
private:
	static const int MAX_THREAD_COUNT;

	/*PREFERED BUFFER SIZE VARIES WITH BUILD CONFIGURATION (x32, x64) AS DIFFERENT AMOUNT OF SYSTEM MEMORY IS GIVEN TO A PROCESS IN EACH CASE*/
	static const int BUFFER_SIZE; 

	static int s_RunningThreads;
	static SafeQueue<FIBITMAP*> s_Queues[];
#ifdef HFR_ORDERING_TEST
	static SafeQueue<int> s_FileNumbersReadTestQueue[];
	static std::vector<int> s_IndexTestResults;
#endif // HFR_ORDERING_TEST

	
	static std::mutex s_CounterMutex;
	static std::mutex s_ConsoleMutex;
	static std::mutex s_ThreadCountMutex;
	static int s_BufferItemCount;

	// load a sequence of images with the provided format and offset. 
	// maxCount specifies the maximum number of images to be loaded by this given call
	// loadOffset specifies the global load offset
	static void LoadSequence(const std::string& formant, int threadIndex, int maxCount, int loadOffset);

public:
	FastImgLoader() = delete;
	FastImgLoader(const FastImgLoader&) = delete;
	FastImgLoader& operator=(const FastImgLoader&) = delete;
	~FastImgLoader();

	static bool LoadImages(const std::string& formant, std::vector<GLuint>* output, int loadOffset, LoadingScreenRenderer* renderer = nullptr);
	static bool LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures);

	inline static bool FileExists(const std::string& name)
	{	
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

};

