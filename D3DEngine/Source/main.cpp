#include <iostream>
#include "checks.h"

int main()
{
	std::cout << "hello world..." << std::endl;
	ENGINE_ERROR("failed call");

	std::cout << "exiting" << std::endl;
	std::cin.get();
	return 0;
}