#pragma once
#include<string>
#include<iostream>

struct ConfigInfo
{
	std::string NameBase;
	float FPS;
	ConfigInfo(const std::string& nameBase, const float fps) :NameBase{ nameBase },FPS{fps}
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

