#pragma once

#include <string>
#include <iostream>

class Configuration
{
public:
	std::string NameBase;
	float FPS;
	double ViewTime;
	int Offset;

	Configuration(const std::string& nameBase, const float fps, double viewTime, int offset) :
		NameBase{ nameBase },
		FPS{ fps },
		ViewTime(viewTime),
		Offset(offset)
	{}

	friend std::ostream& operator<<(std::ostream& os, const Configuration& c);
};

inline std::ostream& operator<<(std::ostream& os, const Configuration& c)
{
	os << "Name base: " << c.NameBase << std::endl <<
		"FPS: " << c.FPS << std::endl <<
		"ViewTime: " << c.ViewTime << std::endl <<
		"Offset: " << c.Offset << std::endl;

	return os;
}