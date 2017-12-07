#pragma once

#include "Configuration.h"

class ConfigurationFactory
{
public:
	ConfigurationFactory() = delete;
	ConfigurationFactory(const ConfigurationFactory&) = delete;
	ConfigurationFactory& operator=(const ConfigurationFactory&) = delete;
	~ConfigurationFactory() { }

	static bool ReadConfig(const std::string& configFile, Configuration** outConfig);
	static bool ParseCmdArgs(int argc, char** args, Configuration** inOutConfig);


};

