#pragma once
#include<vector>
#include<queue>
#include<mutex>
#include <sys/stat.h>

#include "GLTextureLoader.h"
#include "SafeQueue.h"
#define HFR_ORDERING_TEST
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

	static void LoadSequence(const std::string& formant, int startAtIndex);

public:
	FastImgLoader() = delete;
	FastImgLoader(const FastImgLoader&) = delete;
	FastImgLoader& operator=(const FastImgLoader&) = delete;
	~FastImgLoader();

	static bool LoadImages(const std::string& formant, std::vector<GLuint>& output);
	static bool LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures);

	inline static bool FileExists(const std::string& name)
	{
		
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
		

	}

};

