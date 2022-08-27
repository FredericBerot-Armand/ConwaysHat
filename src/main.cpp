#include <iostream>
#include <cstdlib>

#include "FlashMob.h"

void print_usage(char *progname)
{
	std::cout << "Usage : " << progname << " width height delay" << std::endl;
}


int main(int argc, char** argv)
{
	if (argc < 4)
		print_usage(argv[0]);
	else
	{
		int width = std::atoi(argv[1]);
		int height = std::atoi(argv[2]);
		std::chrono::milliseconds delay = std::chrono::milliseconds(std::atoi(argv[3]));
		FlashMob flashMob(width, height);
		flashMob.run(delay);
	}
	return 0;
}