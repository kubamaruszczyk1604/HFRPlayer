#include "FastImgLoader.h"
#include <thread>
#include <iostream>

const int FastImgLoader::MAX_THREAD_COUNT{8};
int FastImgLoader::s_RunningThreads{ 0 };
std::vector<std::queue<FIBITMAP*>>FastImgLoader::s_Queues;
std::vector<std::queue<int>> FastImgLoader::s_TestInts;
std::mutex FastImgLoader::s_CounterMutex;
std::mutex FastImgLoader::s_ConsoleMutex;

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
		
		//std::cout << "Loading file: " << fileName;
		FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName);
		//while (!bitmap)
		{
			//bitmap = GLTextureLoader::LoadImageRAM(fileName);
			//cout << "Trying to read: " << fileName << endl;
		}
		if (bitmap)
		{
			s_ConsoleMutex.lock();
			cout << "OK: " << fileName << endl;
			s_ConsoleMutex.unlock();
			
		}
		
		
		s_Queues[startAtIndex].push(bitmap);
		s_TestInts[startAtIndex].push(counter);

		counter+=MAX_THREAD_COUNT;
		s_CounterMutex.lock();
		tot++;
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
	cout << fileName << endl;


	// DECODE AND LOAD INTO SYSTEM MEMORY

	//check if there is at least one file
	if (!FileExists(fileName)) return false;

	for (int i = 0; i < MAX_THREAD_COUNT; ++i)
	{
		if (FileExists(formant + std::to_string(i) + ".png"))
		{
			
			
        	s_Queues.push_back(queue<FIBITMAP*>());
			s_TestInts.push_back(queue<int>());

			s_RunningThreads++;
			threads.push_back(thread(LoadSequence, formant, i));
		}
	} 

	while (s_RunningThreads) { _sleep(1); } // block until threads are working
	for(int i=0; i < threads.size();++i) { threads[i].join();} // All done. Join.
	cout << "Total images loaded: " << tot << endl;

	threads.clear();
	
	// -> PUSH IMAGES TO GPU IN CORRECT ORDER <-

	/*for (int i = 0; i < s_Queues.size(); ++i)
	{
		cout << "Queue: " << to_string(i) << ", size = " << to_string(s_Queues[i].size()) << endl;
	}*/

	/*std::vector<int> liczby;

    int testIndex = 0;
	for (int i = 0; i < tot; ++i)
	{
	  if (testIndex > (MAX_THREAD_COUNT - 1)) testIndex = 0;

	  int liczba = s_TestInts[testIndex].front();
	  liczby.push_back(liczba);
	  s_TestInts[testIndex].pop();

	 
	  testIndex++;

	}


	for (int i = 0; i < liczby.size(); ++i)
	{
		cout << "Liczba: " << to_string(liczby[i]) << " ogarnieta" << endl;
	}*/


    int queueIndex = 0;
	for (int i = 0; i < tot; ++i)
	{
		if (queueIndex > (MAX_THREAD_COUNT - 1)) queueIndex = 0;
		
		FIBITMAP* bitmap = s_Queues[queueIndex].front();
		output.push_back(GLTextureLoader::PushToGPU(bitmap));
		GLTextureLoader::FreeImageMemory(bitmap);
		s_Queues[queueIndex].pop();
		queueIndex++;

	}

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
