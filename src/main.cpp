#include <iostream>
#include <cstdlib>
#include <string>

#include "FlashMobSimulator.h"



void printUsage(char *progname)
{
	std::cout << "Usage : " << progname << " [-t THRESHOLD] [-c COLOR [COLOR]...] WIDTH HEIGHT DELAY" << std::endl
			  << "with :" << std::endl
			  << "  *  COLOR A value among black, red, green, yellow, blue, purple, cyan and white (Participants changes their hat's colors according to the list order) (default : red white blue)" << std::endl
			  << "  *  THRESHOLD The neigthbours' threshold to change the hat's color of a participant (default : 3)" << std::endl
			  << "  *  WIDTH The width of the grid" << std::endl
			  << "  *  HEIGHT The height of the grid" << std::endl
			  << "  *  DELAY The delay between 2 rooster screams (int millisecond)" << std::endl;
}

int main(int argc, char** argv)
{
	int threshold = 3;
	int argIndex = 1;
	bool endOption = false;
	std::vector<FlashMobSimulator::HatColor> colors;
	try
	{


		/* ************************************************* */
		/* *************** Option management *************** */
		/* ************************************************* */
		while (!endOption)
		{
			std::string currentOptoin = std::string(argv[argIndex]);
			if (currentOptoin == "-t")
			{
				argIndex++;
				threshold = std::atoi(argv[argIndex++]);
			}
			else if (currentOptoin == "-c")
			{
				argIndex++;	
				bool endColor = false;
				while (!endColor)
				{
					std::string currentColor = std::string(argv[argIndex++]);
					if (currentColor == "black")
						colors.push_back(FlashMobSimulator::HatColor::BLACK);
					else if (currentColor == "red")
						colors.push_back(FlashMobSimulator::HatColor::RED);
					else if (currentColor == "green")
						colors.push_back(FlashMobSimulator::HatColor::GREEN);
					else if (currentColor == "yellow")
						colors.push_back(FlashMobSimulator::HatColor::YELLOW);
					else if (currentColor == "blue")
						colors.push_back(FlashMobSimulator::HatColor::BLUE);
					else if (currentColor == "purple")
						colors.push_back(FlashMobSimulator::HatColor::PURPLE);
					else if (currentColor == "cyan")
						colors.push_back(FlashMobSimulator::HatColor::CYAN);
					else if (currentColor == "white")
						colors.push_back(FlashMobSimulator::HatColor::WHITE);
					else
					{
						endColor = true;
						argIndex--;
					}
				}
			}
			else if (currentOptoin == "--")
			{
				argIndex++;
				endOption = true;
			}
			else if (currentOptoin[0] == '-')
				throw 1;
			else
				endOption = true;
		}
		if (argc - argIndex != 3)
			throw 1;
		int width = std::atoi(argv[argIndex++]);
		int height = std::atoi(argv[argIndex++]);
		if (width < 1 || height < 1)
			throw 1;
		std::chrono::milliseconds delay = std::chrono::milliseconds(std::atoi(argv[argIndex++]));
		if (colors.empty())
		{
			colors.push_back(FlashMobSimulator::HatColor::RED);
			colors.push_back(FlashMobSimulator::HatColor::WHITE);
			colors.push_back(FlashMobSimulator::HatColor::BLUE);
		}

		/* ************************************************* */
		/* ****************** Run the App ****************** */
		/* ************************************************* */

		FlashMobSimulator fmSimulator(width, height, colors, threshold);
		fmSimulator.run(delay);

	}
	catch (...)
	{
		printUsage(argv[0]);
		return (-1);
	}

	return 0;
}