#include "ConfigurationFactory.h"
#include <fstream>

#include <iostream>

using namespace std;

#define DEFAULT_NAME ""
#define DEFAULT_FPS -1
#define DEFAULT_OFFSET 0
#define DEFAULT_TIME_LIMIT -1.


bool ConfigurationFactory::ReadConfig(const string & configFile, Configuration** outConfig)
{
	string name = DEFAULT_NAME;
	float fps = DEFAULT_FPS;
	int offset = DEFAULT_OFFSET;
	double timeLimit = DEFAULT_TIME_LIMIT;

	//fl
	ifstream ifs(configFile);
	if (!ifs.is_open()) return false;
	string buffer;
	while (ifs >> buffer)
	{
		if (buffer == "NAME") ifs >> name;
		else if (buffer == "FPS") ifs >> fps;
		else if (buffer == "OFFSET") ifs >> offset;
	}
	if (timeLimit <= 0)
	{
		timeLimit = DBL_MAX;
	}
	if (name == "") return false;

	*outConfig = new Configuration(name, fps, DBL_MAX, offset);
	return true;
}

bool ConfigurationFactory::ParseCmdArgs(int argc, char ** args, Configuration ** inOutConfig)
{
	string name = DEFAULT_NAME;
	float fps = DEFAULT_FPS;
	int offset = DEFAULT_OFFSET;
	double timeLimit = DEFAULT_TIME_LIMIT;

	// not enough command line arguments
	if (argc < 2)
	{
		return false;
	}

	std::cout << "ARGS PROVIDED..." << std::endl;

	if (argc > 1) { name = std::string(args[1]); } // name base (formant) provided
	if (argc > 2) // fps provided..
	{
		try // check if floating point number.
		{
			fps = std::stof(std::string(args[2]));
		}
		catch (std::exception& e) // not a float
		{
			fps = DEFAULT_FPS;
			std::cout << "Incorrect FPS argument" << std::endl;
		}
	}
	if (argc > 3) // time limit provided
	{
		try // check if floating point number.
		{
			timeLimit = std::stod(std::string(args[3]));
		}
		catch (std::exception& e) // not a double
		{
			timeLimit = DEFAULT_TIME_LIMIT;
			std::cout << "Incorrect view time limit argument" << std::endl;
		}
	}
	if (argc > 4) // offset provided
	{
		try // check if int
		{
			offset = std::stoi(std::string(args[4]));
		}
		catch (std::exception& e) // not an int
		{
			offset = DEFAULT_OFFSET;
			std::cout << "No offset provided" << std::endl;
		}
	}

	if (timeLimit <= 0)
	{
		timeLimit = DBL_MAX;
	}

	*inOutConfig = new Configuration(name, fps, timeLimit, offset);
	return true;
}
