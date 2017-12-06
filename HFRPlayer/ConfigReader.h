#pragma once
#include<string>
#include<iostream>

struct ConfigInfo
{
	std::string NameBase;
	float FPS;
	double ViewTime;
	int Offset;

	ConfigInfo(const std::string& nameBase, const float fps, double viewTime, int offset) : 
		NameBase{ nameBase }, 
		FPS{fps}, 
		ViewTime(viewTime),
		Offset(offset)
	{}
};


class ConfigReader
{
public:
	ConfigReader() = delete;
	ConfigReader(const ConfigReader&) = delete;
	ConfigReader& operator=(const ConfigReader&) = delete;
	~ConfigReader();

	static bool ReadConfig(const std::string& configFile, ConfigInfo*& outConfig);

};

