// MAIN ENTRY HERE 
#include "ExperimentSocketManager.h"
#include "Renderer.h"
#include "FastImgLoader.h"
#include "ConfigReader.h"
#include <exception>



Stopwatch _BenchmarkTimer;


int main(int argc, char** args)
{
	ConfigInfo* conf{ nullptr };
	if (argc < 2) // no cmd line args provided - READ CONFIG
	{
		std::cout << "NO ARGS PROVIDED... READING CONFIG: ";
		if (!ConfigReader::ReadConfig("config.txt", conf))
		{
			std::cout << " FAILED" << std::endl;
			std::cout << " HFR PLAYER WILL QUIT NOW." << std::endl;
			return 0;
		}
		else { std::cout << " OK" << std::endl; }
	}

	else // Args provided - USE PROVIDED ARGS
	{
		std::string name = "";
		float fps = -1;
		std::cout << "ARGS PROVIDED..." << std::endl;
		if (argc > 1) { name = std::string(args[1]); } // name base (formant) provided
		if (argc > 2) // fps provided..
		{
			try // check if floating point number.
			{
				fps = std::stof(std::string(args[2]));
				std::cout << fps << std::endl;
			}
			catch (std::exception& e) // not a float
			{
				fps = -1;
				std::cout << "Incorrect FPS argument --> NO FPS LIMIT WILL BE APPLIED!" << std::endl;
			}
		}
		conf = new ConfigInfo(std::string(args[1]), fps);
	}

	std::cout << "NAME BASE: " << conf->NameBase << ", FPS REQUEST: " << conf->FPS << std::endl;

	// TODO: consider loading port from config file. For now, fixing it to 30000
	Networking::ExperimentSocketManager::initialise(30000);

	///////////////  IMAGE LOADING AND RENDERING ////////////////

	// Init OpenGL
	Renderer::Init(256, 144, "FPS", false); 
	Renderer::SetFPS(conf->FPS);

	//Read in images
	std::vector<GLuint> imagesIDs;
	_BenchmarkTimer.Start();
	if (!FastImgLoader::LoadImages(conf->NameBase,imagesIDs)) // if there is not a single image file - quit.
	{
		std::cout << "No file(s) found!" << std::endl;
		return 0;
	}
	std::cout << "Images Loading took: " << _BenchmarkTimer.ElapsedTime() << " seconds." << std::endl;
	_BenchmarkTimer.Stop();

	delete conf;
	conf = nullptr;

	// Pass images to the renderer
	Renderer::SetPictures(imagesIDs);

	// Renderer loop(blocking)
	Renderer::Run();

	Networking::ExperimentSocketManager::cleanup();

	std::cout << "Deleting textures..." << std::endl;
	// free all textures
	glDeleteTextures(imagesIDs.size(), &imagesIDs[0]);


	return 0;
}

