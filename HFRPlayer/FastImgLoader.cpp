#include "FastImgLoader.h"
#include "Stopwatch.h"
#include <thread>
#include <iostream>

const int FastImgLoader::MAX_THREAD_COUNT{4};
const int FastImgLoader::BUFFER_SIZE{ 508 };
int FastImgLoader::s_RunningThreads{ 0 };
SafeQueue<FIBITMAP*> FastImgLoader::s_Queues[MAX_THREAD_COUNT];
std::mutex FastImgLoader::s_CounterMutex;
std::mutex FastImgLoader::s_ConsoleMutex;
std::mutex FastImgLoader::s_ThreadCountMutex;
int FastImgLoader::s_BufferItemCount{ 0 };

#ifdef HFR_ORDERING_TEST
SafeQueue<int> FastImgLoader::s_FileNumbersReadTestQueue[MAX_THREAD_COUNT];
std::vector<int> FastImgLoader::s_IndexTestResults;
#endif // HFR_ORDERING_TEST

using namespace std;

FastImgLoader::~FastImgLoader()
{
}
void FastImgLoader::LoadSequence(const string& formant, int threadIndex, int maxCount, int loadOffset)
{
	int counter = threadIndex + loadOffset;
	int stopIndex = counter + maxCount * MAX_THREAD_COUNT;
	string fileName = formant + std::to_string(counter) + ".png";

	while (counter < stopIndex && FileExists(fileName))
	{	
		while (s_BufferItemCount > BUFFER_SIZE) // this is to prevent buffer overflow. Gives consumer time to process items in the queues
		{
			_sleep(0);
		}
		FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName); // <- THIS LINE TAKES MOST TIME (about 46%)

		if (bitmap == nullptr)
		{
			s_ConsoleMutex.lock();
			cout << "ERROR! FAILED TO ALLOCATE: " << fileName << endl;
			s_ConsoleMutex.unlock();
		}

		s_Queues[threadIndex].enqueue(bitmap);
#ifdef HFR_ORDERING_TEST
		s_FileNumbersReadTestQueue[threadIndex].enqueue(counter);
#endif // HFR_ORDERING_TEST
		counter+=MAX_THREAD_COUNT;
		s_CounterMutex.lock();
		s_BufferItemCount++;
		s_CounterMutex.unlock();
		fileName = formant + std::to_string(counter) + ".png";
	}
	s_ThreadCountMutex.lock();
	s_RunningThreads--;
	s_ThreadCountMutex.unlock();

}

bool FastImgLoader::LoadImages(const string& formant, vector<GLuint>& output, int loadOffset, LOADING_PROGRESS_CALLBACK progressCallbackFunction)
{
	output.clear();
	string fileName = formant + std::to_string(loadOffset) + ".png";
	cout << "Starting from file: " << fileName << endl;

	//check if there is at least one file
	if (!FileExists(fileName)) return false;

	// find how many images can we fit in the GPU memory (assuming 10GB)
	FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName);
	int w = FreeImage_GetWidth(bitmap);
	int h = FreeImage_GetHeight(bitmap);
	uint64_t frameSize = w * h * 3;
	int maxFrameCount = (int)(8ULL * 1024ULL * 1024ULL * 1024ULL / frameSize);
	GLTextureLoader::FreeImageMemory(bitmap);

	// DECODE AND LOAD INTO SYSTEM MEMORY -- PROIDUCER THREADS SPAWNED HERE ---
	vector<thread> threads;
	for (int i = 0; i < MAX_THREAD_COUNT; ++i)
	{
		if (FileExists(formant + std::to_string(i + loadOffset) + ".png"))
		{		
		    s_ThreadCountMutex.lock();
			s_RunningThreads++;
			s_ThreadCountMutex.unlock();
			threads.push_back(thread(LoadSequence, formant, i, maxFrameCount / MAX_THREAD_COUNT, loadOffset));
		}
	} 
	_sleep(10);
	int totalConsumed = 0;
	// -> PUSH IMAGES TO GPU IN CORRECT ORDER <-  --- CONSUMER LOOP ---
    int queueIndex = 0;
    while(s_RunningThreads > 0 || s_BufferItemCount > 0)
	{
		if (queueIndex > (MAX_THREAD_COUNT - 1)) queueIndex = 0;
		
		FIBITMAP* bitmap = nullptr;
		while (!s_Queues[queueIndex].dequeue(bitmap))
		{
			_sleep(0);
		}
		output.push_back(GLTextureLoader::PushToGPU(bitmap));
#ifdef HFR_ORDERING_TEST
		int index;
		while (!s_FileNumbersReadTestQueue[queueIndex].dequeue(index))
		{
			_sleep(0);
		}
		s_IndexTestResults.push_back(index);
#endif // HFR_ORDERING_TEST
		GLTextureLoader::FreeImageMemory(bitmap);
		s_CounterMutex.lock();
		s_BufferItemCount--; 
		s_CounterMutex.unlock();
		queueIndex++;
		totalConsumed++;
		if (progressCallbackFunction) progressCallbackFunction(totalConsumed);
		glfwPollEvents();
	}
	
	//while (s_RunningThreads) { _sleep(0); } // block until threads are working 
	for (int i = 0; i < threads.size(); ++i) { threads[i].join(); } // All done. Join.
	cout << "Reading finished. Total loaded: " << to_string(totalConsumed) << endl;
#ifdef HFR_ORDERING_TEST
	std::cout << "FILES WERE PUSHED TO GPU IN FOLLOWING ORDER: " << std::endl;
	for (int i = 0; i < s_IndexTestResults.size(); ++i)
	{
		std::cout << std::to_string(s_IndexTestResults[i]) << ", ";
	}
	cout << endl;
	s_IndexTestResults.clear();
#endif // HFR_ORDERING_TEST
	return true;
}

bool FastImgLoader::LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures)
{
	unsigned counter = 0;
	string fileName = formant + std::to_string(counter) + ".png";
	cout << fileName;
	//check if there is at least one file
	if (!FileExists(fileName)) { return false;}

	while (FileExists(fileName))
	{
		std::cout << "Loading file: " << fileName;
		FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName);
		textures.push_back(GLTextureLoader::PushToGPU(bitmap));
		GLTextureLoader::FreeImageMemory(bitmap);
		std::cout << " LOADED" << std::endl;
		counter++;
		fileName = formant + std::to_string(counter) + ".png";
	}

	std::cout << "Loaded " << (counter) << " files." << std::endl;
	return true;
}

