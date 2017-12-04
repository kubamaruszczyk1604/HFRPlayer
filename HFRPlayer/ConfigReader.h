#pragma once
#include<string>
#include<iostream>

struct ConfigInfo
{
	std::string NameBase;
	float FPS;
	double ViewTime;

	ConfigInfo(const std::string& nameBase, const float fps, double viewTime) : NameBase{ nameBase }, FPS{fps}, ViewTime(viewTime)
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

