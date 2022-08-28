#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>

#include "FlashMobSimulator.h"

FlashMobSimulator::FlashMobSimulator(int width, int height, std::vector<HatColor>& colors, int threshold) : 
	width(width),
	height(height),
	colors(colors),
	threshold(threshold)
{
	createGrid();
	std::srand(std::time(nullptr));
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = static_cast<HatColor>(std::rand()/((RAND_MAX + 1u)/colors.size()));
	}
}


FlashMobSimulator::FlashMobSimulator(const FlashMobSimulator& flashMobSimulator) :
	width(flashMobSimulator.getWidth()),
	height(flashMobSimulator.getHeight()),
	colors(flashMobSimulator.colors),
	threshold(flashMobSimulator.threshold)
{
	createGrid();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = flashMobSimulator.getHat(i, j);
	}
}


FlashMobSimulator::~FlashMobSimulator()
{
	reset();
}

FlashMobSimulator& FlashMobSimulator::operator=(const FlashMobSimulator& flashMobSimulator)
{
	if (width != flashMobSimulator.getWidth() || height != flashMobSimulator.getHeight())
	{
		reset();
		width = flashMobSimulator.getWidth();
		height = flashMobSimulator.getHeight();
		createGrid();
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = flashMobSimulator.getHat(i, j);
	}
	return *this;
}

int FlashMobSimulator::getWidth() const
{
	return width;
}

int FlashMobSimulator::getHeight() const
{
	return height;
}

int FlashMobSimulator::getHat(int x, int y) const
{
	return grid[y][x];
}

void FlashMobSimulator::run(std::chrono::milliseconds delay)
{
	printf("\033[s");
	printGrid();
	while(computeNextState())
	{
		std::this_thread::sleep_for(delay);
		printGrid();
	}
}

void FlashMobSimulator::reset()
{
	for (int i = 0; i < height; i++)
	{
		delete[] grid[i];
		delete[] nextState[i];
	}
	delete[] grid;
	delete[] nextState;
	width = -1;
	height = -1;
}

void FlashMobSimulator::createGrid()
{
	grid = new int*[height];
	nextState = new int*[height];
	for (int i = 0; i < height; i++)
	{
		grid[i] = new int[width];
		nextState[i] = new int[width];
	}
}

int FlashMobSimulator::countNeighborsHats(int x, int y, int hatColor)
{
	int result = 0;
	if (y > 0)
	{
		result += (x > 0 && getHat(x - 1, y - 1) == hatColor ? 1 : 0);
		result += (getHat(x, y - 1) == hatColor ? 1 : 0);
		result += (x < getWidth() - 1 && getHat(x + 1, y - 1) == hatColor ? 1 : 0);
	}
	result += (x > 0 && getHat(x - 1, y) == hatColor ? 1 : 0);
	result += (x < getWidth() - 1 && getHat(x + 1, y) == hatColor ? 1 : 0);
	if ( y < getHeight() - 1)
	{
		result += (x > 0 && getHat(x - 1, y + 1) == hatColor ? 1 : 0);
		result += (getHat(x , y + 1) == hatColor ? 1 : 0);
		result += (x < getWidth() - 1 && getHat(x + 1, y + 1) == hatColor ? 1 : 0);	
	}
	return result;
}

int FlashMobSimulator::computeNextState()
{
	int countChange = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int nextHat = (getHat(x, y) + 1) % colors.size();
			if (countNeighborsHats(x, y, nextHat) >= threshold)
			{
				nextState[y][x] = nextHat;
				countChange++;
			}
			else
				nextState[y][x] = getHat(x, y);
		}
	}
	int** tmpSwap = grid;
	grid = nextState;
	nextState = tmpSwap;
	return (countChange);
}

void FlashMobSimulator::printGrid()
{
	std::stringstream ss;
	HatColor lastColor = HatColor::NONE;
	if (firstPrint)
		firstPrint = false;
	else
	{
		for (int i = 0; i < getHeight(); i++)
			ss << "\033[A";
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			HatColor currentHat = colors[getHat(x, y)];
			if (lastColor == HatColor::NONE || lastColor != currentHat)
				ss << "\033[" << 40 + static_cast<int>(currentHat) << 'm';
			ss << ' ';
		}
		ss << "\033[0m" << std::endl;
		lastColor = HatColor::NONE;
	}
	std::cout << ss.str();
}
