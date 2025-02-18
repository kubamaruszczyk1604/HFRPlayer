// MAIN ENTRY HERE 
#include "ExperimentSocketManager.h"
#include "Renderer.h"
#include "ConfigurationFactory.h"
#include "PathResolver.h"

#include <exception>
#include <iostream>


#define FULL_SCREEN 0


int main(int argc, char** args)
{
	PathResolver::initialise(args[0]);

	// find configuration
	Configuration* conf{ nullptr };
	if (!ConfigurationFactory::ParseCmdArgs(argc, args, &conf))
	{
		if (!ConfigurationFactory::ReadConfig(PathResolver::getPathToExe() +  "config.txt", &conf))
		{
			std::cout << "no argument or config file provided";
			return 1;
		}
	}
	std::cout << *conf;

	// initialise network
	Networking::ExperimentSocketManager::initialise(30000);

	// Init OpenGL
	Renderer::Init(2560, 1440, "FPS", FULL_SCREEN);
	Renderer::SetFPS(conf->FPS);
	Renderer::LoadTextures(conf->NameBase, conf->Offset);
	Renderer::SetMaximumViewTime(conf->ViewTime);

	// Renderer loop(blocking)
	Renderer::Run();

	Networking::ExperimentSocketManager::cleanup();
	delete conf;
	return 0;
}

