#include "Gadgets.h"

std::string find_expender(std::string name)
{
	std::string token;
	std::string t_name = name;
	std::stringstream ss(t_name);

	while (std::getline(ss, token, '.'));
	return token;
}