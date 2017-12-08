#pragma once

#include <string>

// resolve location of exe file
class PathResolver
{
public:

	static void initialise(char* arg0);
	static void cleanup() { s_path.clear(); }

	inline static const std::string& getPathToExe() { return s_path; }

private:
	static std::string s_path;
};