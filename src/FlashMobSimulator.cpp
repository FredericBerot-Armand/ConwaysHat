#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>

#include "FlashMobSimulator.h"

FlashMobSimulator::FlashMobSimulator(int width, int height, std::vector<HatColor>& colors) :
	width_(width),
	height_(height),
	colors_(colors)
{
	createGrid();
	std::srand(std::time(nullptr));
	for (int i = 0; i < height_; i++)
	{
		for (int j = 0; j < width_; j++)
			grid_[i][j] = static_cast<HatColor>(std::rand()/((RAND_MAX + 1u)/colors_.size()));
	}
}

FlashMobSimulator::FlashMobSimulator(const FlashMobSimulator& flashMobSimulator) :
	width_(flashMobSimulator.getWidth()),
	height_(flashMobSimulator.getHeight()),
	colors_(flashMobSimulator.colors_)
{
	createGrid();
	for (int i = 0; i < height_; i++)
	{
		for (int j = 0; j < width_; j++)
			grid_[i][j] = flashMobSimulator.getHat(i, j);
	}
}


FlashMobSimulator::~FlashMobSimulator()
{
	reset();
}

FlashMobSimulator& FlashMobSimulator::operator=(const FlashMobSimulator& flashMobSimulator)
{
	if (width_ != flashMobSimulator.getWidth() || height_ != flashMobSimulator.getHeight())
	{
		reset();
		width_ = flashMobSimulator.getWidth();
		height_ = flashMobSimulator.getHeight();
		createGrid();
	}
	for (int i = 0; i < height_; i++)
	{
		for (int j = 0; j < width_; j++)
			grid_[i][j] = flashMobSimulator.getHat(i, j);
	}
	return *this;
}

int FlashMobSimulator::getWidth() const
{
	return width_;
}

int FlashMobSimulator::getHeight() const
{
	return height_;
}

int FlashMobSimulator::getHat(int x, int y) const
{
	return grid_[y][x];
}

void FlashMobSimulator::run(int threshold, std::chrono::milliseconds delay, bool torus)
{
	int (FlashMobSimulator::*countFun)(int, int, int) = (torus ? &FlashMobSimulator::countNeighborsHatsOnTorus : &FlashMobSimulator::countNeighborsHats);
	printf("\033[s");
	printGrid();

	while(computeNextState(countFun, threshold))
	{
		std::this_thread::sleep_for(delay);
		printGrid();
	}
}

void FlashMobSimulator::reset()
{
	for (int i = 0; i < height_ + 2; i++)
	{
		delete[] grid_[i];
		delete[] nextState_[i];
	}
	delete[] grid_;
	delete[] nextState_;
	width_ = -1;
	height_ = -1;
}

void FlashMobSimulator::createGrid()
{
	grid_ = new int*[height_ + 2];
	nextState_ = new int*[height_ + 2];
	for (int i = 0; i < height_ + 2; i++)
	{
		grid_[i] = new int[width_ + 2];
		nextState_[i] = new int[width_ + 2];
	}
	for (int i = 0; i < height_ + 2; i++)
	{
		grid_[i][0] = -1;
		nextState_[i][0] = -1;
		grid_[i][width_ + 1] = -1;
		nextState_[i][width_ + 1] = -1;
	}
	for (int i = 0; i < width_ + 2; i++)
	{
		grid_[0][i] = -1;
		nextState_[0][i] = -1;
		grid_[height_ + 1][i] = -1;
		nextState_[height_ + 1][i] = -1;
	}
}

int FlashMobSimulator::countNeighborsHats(int x, int y, int hatColor)
{
	int result = 0;
	result += (getHat(x - 1, y - 1) == hatColor ? 1 : 0);
	result += (getHat(x, y - 1) == hatColor ? 1 : 0);
	result += (getHat(x + 1, y - 1) == hatColor ? 1 : 0);
	result += (getHat(x - 1, y) == hatColor ? 1 : 0);
	result += (getHat(x + 1, y) == hatColor ? 1 : 0);
	result += (getHat(x - 1, y + 1) == hatColor ? 1 : 0);
	result += (getHat(x , y + 1) == hatColor ? 1 : 0);
	result += (getHat(x + 1, y + 1) == hatColor ? 1 : 0);
	return result;
}

int FlashMobSimulator::countNeighborsHatsOnTorus(int x, int y, int hatColor)
{
	int prevX = (x == 1 ? width_ : x - 1);
	int nextX = (x == width_ ? 1 : x + 1);
	int prevY = (y == 1 ? height_ : y - 1);
	int nextY = (y == height_ ? 1 : y + 1);
	int result = 0;
	result += (getHat(prevX, prevY) == hatColor ? 1 : 0);
	result += (getHat(x, prevY) == hatColor ? 1 : 0);
	result += (getHat(nextX, prevY) == hatColor ? 1 : 0);
	result += (getHat(prevX, y) == hatColor ? 1 : 0);
	result += (getHat(nextX, y) == hatColor ? 1 : 0);
	result += (getHat(prevX, nextY) == hatColor ? 1 : 0);
	result += (getHat(x , nextY) == hatColor ? 1 : 0);
	result += (getHat(nextX, nextY) == hatColor ? 1 : 0);
	return result;
}


int FlashMobSimulator::computeNextState(int (FlashMobSimulator::*countFun)(int, int, int), int threshold)
{
	int countChange = 0;
	for (int y = 1; y < height_ + 1; y++)
	{
		for (int x = 1; x < width_ + 1; x++)
		{
			int nextHat = (getHat(x, y) + 1) % colors_.size();
			if ((*this.*countFun)(x, y, nextHat) >= threshold)
			{
				nextState_[y][x] = nextHat;
				countChange++;
			}
			else
				nextState_[y][x] = getHat(x, y);
		}
	}
	int** tmpSwap = grid_;
	grid_ = nextState_;
	nextState_ = tmpSwap;
	return countChange;
}

void FlashMobSimulator::printGrid()
{
	std::stringstream ss;
	HatColor lastColor = HatColor::NONE;
	if (firstPrint_)
		firstPrint_ = false;
	else
	{
		for (int i = 0; i < getHeight(); i++)
			ss << "\033[A";
	}
	for (int y = 1; y < height_ + 1; y++)
	{
		for (int x = 1; x < width_ + 1; x++)
		{
			HatColor currentHat = colors_[getHat(x, y)];
			if (lastColor == HatColor::NONE || lastColor != currentHat)
				ss << "\033[" << 40 + static_cast<int>(currentHat) << 'm';
			ss << ' ';
		}
		ss << "\033[0m" << std::endl;
		lastColor = HatColor::NONE;
	}
	std::cout << ss.str();
}
