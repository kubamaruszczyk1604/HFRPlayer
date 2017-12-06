#include "ConfigReader.h"
#include <fstream>

using namespace std;

ConfigReader::~ConfigReader()
{

}

bool ConfigReader::ReadConfig(const string & configFile, ConfigInfo*& outConfig)
{
	//fl
	ifstream ifs(configFile);
	if (!ifs.is_open()) return false;
	string name = "";
	float fps = -1;
	int offset;
	string buffer;
	while (ifs >> buffer)
	{
		if (buffer == "NAME") ifs >> name;
		else if (buffer == "FPS") ifs >> fps;
		else if (buffer == "OFFSET") ifs >> offset;
	}
	if (name == "") return false;

	outConfig = new ConfigInfo(name, fps, DBL_MAX, offset);
	
	return true;
}
