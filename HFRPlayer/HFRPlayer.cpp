// MAIN ENTRY HERE 
#include "ExperimentSocketManager.h"
#include "Renderer.h"
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
		double timeLimit = -1;
		int offset = 0;
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
		if (argc > 3) // time limit provided
		{
			try // check if floating point number.
			{
				timeLimit = std::stod(std::string(args[3]));
				std::cout << timeLimit << std::endl;
			}
			catch (std::exception& e) // not a double
			{
				timeLimit = -1;
				std::cout << "Incorrect view time limit argument --> NO dimming!" << std::endl;
			}
		}
		if (argc > 4) // offset provided
		{
			try // check if int
			{
				offset = std::stoi(std::string(args[4]));
				std::cout << offset << std::endl;
			}
			catch (std::exception& e) // not an int
			{
				offset = 0;
				std::cout << "No offset provided. Defaulting to 0" << std::endl;
			}
		}

		if (timeLimit <= 0)
		{
			timeLimit = DBL_MAX;
		}

		conf = new ConfigInfo(std::string(args[1]), fps, timeLimit, offset);
	}

	std::cout << "NAME BASE: " << conf->NameBase << ", FPS REQUEST: " << conf->FPS << std::endl;

	// TODO: consider loading port from config file. For now, fixing it to 30000
	Networking::ExperimentSocketManager::initialise(30000);

	///////////////  IMAGE LOADING AND RENDERING ////////////////

	// Init OpenGL
	Renderer::Init(2560, 1440, "FPS", true); 
	Renderer::SetFPS(conf->FPS);
	Renderer::LoadTextures(conf->NameBase, conf->Offset);
	Renderer::SetMaximumViewTime(conf->ViewTime);

	// Renderer loop(blocking)
	Renderer::Run();

	Networking::ExperimentSocketManager::cleanup();
	delete conf;
	conf = nullptr;
	return 0;
}

