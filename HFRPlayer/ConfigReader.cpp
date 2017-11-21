#include "ConfigReader.h"
#include <fstream>




ConfigReader::~ConfigReader()
{
}

using namespace std;

bool ConfigReader::ReadConfig(const string & configFile, ConfigInfo*& outConfig)
{
	//
	ifstream ifs(configFile);
	if (!ifs.is_open()) return false;
	string name = "";
	float fps = -1;
	string buffer;
	while (ifs >> buffer)
	{
		if (buffer == "NAME") ifs >> name;
		else if (buffer == "FPS") ifs >> fps;
	}

	if (fps < 0) return false;
	if (name == "") return false;

	outConfig = new ConfigInfo(name, fps);
	
	return true;
}
