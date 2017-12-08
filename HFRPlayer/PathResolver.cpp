#include "PathResolver.h"

std::string PathResolver::s_path = "";

void PathResolver::initialise(char * arg0)
{
	auto temp = std::string(arg0);
	s_path = temp.substr(0, temp.rfind('\\') + 1);
}
