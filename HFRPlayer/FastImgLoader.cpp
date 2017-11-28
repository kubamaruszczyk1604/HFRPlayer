#include "FastImgLoader.h"
#include <thread>
#include <iostream>

const int FastImgLoader::MAX_THREAD_COUNT{8};
int FastImgLoader::s_RunningThreads{ 0 };
SafeQueue<FIBITMAP*> FastImgLoader::s_Queues[MAX_THREAD_COUNT];
//std::vector<SafeQueue<int>> FastImgLoader::s_TestInts;
std::mutex FastImgLoader::s_CounterMutex;
std::mutex FastImgLoader::s_ConsoleMutex;
int FastImgLoader::s_ActiveCount{ 0 };
using namespace std;

FastImgLoader::~FastImgLoader()
{
}


int tot = 0;
void FastImgLoader::LoadSequence(const string& formant, int startAtIndex)
{
	int counter = startAtIndex;
	string fileName = formant + std::to_string(counter) + ".png";
	while (FileExists(fileName))
	{	
		while (s_ActiveCount > 125)
		{
			_sleep(0);
		}
		FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName);
	
		if (bitmap == nullptr)
		{
			s_ConsoleMutex.lock();
			cout << "ERROR: " << fileName << endl;
			s_ConsoleMutex.unlock();
			
		}

		s_Queues[startAtIndex].enqueue(bitmap);
		counter+=MAX_THREAD_COUNT;
		s_CounterMutex.lock();
		s_ActiveCount++;
		s_CounterMutex.unlock();
		fileName = formant + std::to_string(counter) + ".png";
	}

	//std::cout << "Loaded all starting at index " << to_string(startAtIndex) << endl;// "Queue size: " << to_string(outQueue.size()) << endl;
	s_RunningThreads--;
	
}

bool FastImgLoader::LoadImages(const string& formant, vector<GLuint>& output)
{

	vector<thread> threads;
	string fileName = formant + std::to_string(0) + ".png";
	cout << "Starting from file: " << fileName << endl;


	// DECODE AND LOAD INTO SYSTEM MEMORY

	//check if there is at least one file
	if (!FileExists(fileName)) return false;

	for (int i = 0; i < MAX_THREAD_COUNT; ++i)
	{
		if (FileExists(formant + std::to_string(i) + ".png"))
		{		
			s_RunningThreads++;
			threads.push_back(thread(LoadSequence, formant, i));
		}
	} 

	_sleep(100);
	
	// -> PUSH IMAGES TO GPU IN CORRECT ORDER <-
    int queueIndex = 0;
	for (int i = 0; i < 508; ++i)
	{
		if (queueIndex > (MAX_THREAD_COUNT - 1)) queueIndex = 0;
		
		FIBITMAP* bitmap = nullptr;
		while (!s_Queues[queueIndex].dequeue(bitmap))
		{
			_sleep(0);
		}
		output.push_back(GLTextureLoader::PushToGPU(bitmap));
		GLTextureLoader::FreeImageMemory(bitmap);
		s_CounterMutex.lock();
		s_ActiveCount--;
		s_CounterMutex.unlock();
		queueIndex++;

	}
	
	//while (s_RunningThreads) { _sleep(0); } // block until threads are working 
	for (int i = 0; i < threads.size(); ++i) { threads[i].join(); } // All done. Join.
	cout << "All in" << endl;

}

bool FastImgLoader::LoadImagesSingleThread(const std::string& formant, std::vector<GLuint>& textures)
{
	unsigned counter = 0;
	string fileName = formant + std::to_string(counter) + ".png";
	cout << fileName;


	//check if there is at least one file
	if (!FileExists(fileName))
	{
		return false;
	}

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

bool FastImgLoader::FileExists(const std::string & name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
