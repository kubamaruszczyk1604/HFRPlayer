#include "FastImgLoader.h"
#include <thread>
#include <iostream>

const int FastImgLoader::MAX_THREAD_COUNT{ 8 };
using namespace std;



FastImgLoader::~FastImgLoader()
{
}
int tot = 0;
void FastImgLoader::LoadSequence(const string& formant, int startAtIndex, queue<FIBITMAP*>& outQueue)
{
	cout << "QUEUE: " + to_string(startAtIndex) << " created" << std::endl;

	int counter = startAtIndex;
	string fileName = formant + std::to_string(counter) + ".png";

	while (FileExists(fileName))
	{
		std::cout << "Loading file: " << fileName;
		FIBITMAP* bitmap = GLTextureLoader::LoadImageRAM(fileName);
		outQueue.push(bitmap);
	//	textures.push_back(GLTextureLoader::PushToGPU(bitmap));
		GLTextureLoader::FreeImageMemory(bitmap);
		//textures.push_back(GLTextureLoader::LoadTexture(fileName));
		std::cout << " LOADED" << std::endl;
		counter+=MAX_THREAD_COUNT;
		tot++;
		fileName = formant + std::to_string(counter) + ".png";
	}

	std::cout << "Loaded all starting at index " << to_string(startAtIndex) << endl;

}

bool FastImgLoader::LoadImages(const string& formant, vector<GLuint>& output)
{
	vector<queue<FIBITMAP*>> queues;
	vector<thread> threads;


	string fileName = formant + std::to_string(0) + ".png";
	cout << fileName << endl;

	//check if there is at least one file
	if (!FileExists(fileName)) return false;

	for (int i = 0; i < MAX_THREAD_COUNT; ++i)
	{
		if (FileExists(formant + std::to_string(i) + ".png"))
		{
			queue<FIBITMAP*> q;
			queues.push_back(q);
			LoadSequence(formant, i, q);
		}
	} 

	cout << "Total images loaded: " << tot << endl;

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
		//textures.push_back(GLTextureLoader::LoadTexture(fileName));
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
