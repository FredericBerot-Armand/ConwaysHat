#include <chrono>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <thread>

#include "FlashMob.h"

FlashMob::FlashMob(int width, int height) : 
	width(width),
	height(height)
{
	CreateGrid();
	std::srand(std::time(nullptr));
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = static_cast<HatColor>(std::rand()/((RAND_MAX + 1u)/3));
	}
}

FlashMob::FlashMob(const FlashMob& flashMob) :
	width(flashMob.GetWidth()),
	height(flashMob.GetHeight())
{
	CreateGrid();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = flashMob.GetHat(i, j);
	}
}

FlashMob::~FlashMob()
{
	Reset();
}

FlashMob& FlashMob::operator=(const FlashMob& flashMob)
{
	if (width != flashMob.GetWidth() || height != flashMob.GetHeight())
	{
		Reset();
		width = flashMob.GetWidth();
		height = flashMob.GetHeight();
		CreateGrid();
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			grid[i][j] = flashMob.GetHat(i, j);
	}
	return *this;
}

int FlashMob::GetWidth() const
{
	return width;
}

int FlashMob::GetHeight() const
{
	return height;
}


FlashMob::HatColor &FlashMob::GetHat(int x, int y) const
{
	return grid[y][x];
}

void FlashMob::Run(std::chrono::milliseconds delay)
{
	printf("\033[s");
	Print();

	while(ComputeNextState())
	{
		std::this_thread::sleep_for(delay);
		Print();
	}
}

void FlashMob::Reset()
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

void FlashMob::CreateGrid()
{
	grid = new HatColor*[height];
	nextState = new HatColor*[height];
	for (int i = 0; i < height; i++)
	{
		grid[i] = new HatColor[width];
		nextState[i] = new HatColor[width];
	}
}

int FlashMob::CountNeighborsHats(int x, int y, HatColor hatColor)
{
	int result = 0;
	if (y > 0)
	{
		result += (x > 0 && GetHat(x - 1, y - 1) == hatColor ? 1 : 0);
		result += (GetHat(x, y - 1) == hatColor ? 1 : 0);
		result += (x < GetWidth() - 1 && GetHat(x + 1, y - 1) == hatColor ? 1 : 0);
	}
	result += (x > 0 && GetHat(x - 1, y) == hatColor ? 1 : 0);
	result += (x < GetWidth() - 1 && GetHat(x + 1, y) == hatColor ? 1 : 0);
	if ( y < GetHeight() - 1)
	{
		result += (x > 0 && GetHat(x - 1, y + 1) == hatColor ? 1 : 0);
		result += (GetHat(x , y + 1) == hatColor ? 1 : 0);
		result += (x < GetWidth() - 1 && GetHat(x + 1, y + 1) == hatColor ? 1 : 0);	
	}
	return result;
}

int FlashMob::ComputeNextState()
{
	int countChange = 0;
	//TODO multithreading
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			HatColor nextHat = GetHat(x, y);
			switch (nextHat)
			{
				case HatColor::BLUE:
					if (CountNeighborsHats(x, y, HatColor::RED) >= 3)
						nextHat = HatColor::RED;
					break;
				case HatColor::WHITE:
					if (CountNeighborsHats(x, y, HatColor::BLUE) >= 3)
						nextHat = HatColor::BLUE;
					break;
				case HatColor::RED:
					if (CountNeighborsHats(x, y, HatColor::WHITE) >= 3)
						nextHat = HatColor::WHITE;
					break;
			}
			if (nextHat != GetHat(x, y))
				countChange++;
			nextState[y][x] = nextHat;
		}
	}
	HatColor** tmpSwap = grid;
	grid = nextState;
	nextState = tmpSwap;
	return (countChange);
}

void FlashMob::Print()
{
	std::stringstream ss;
	int lastColor = -1;
	if (firstPrint)
		firstPrint = false;
	else
	{
		for (int i = 0; i < GetHeight(); i++)
			ss << "\033[A";
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			switch (GetHat(x, y))
			{
				case HatColor::BLUE:
					if (lastColor == -1 || lastColor != static_cast<int>(HatColor::BLUE))
						ss << "\033[44m";
					ss << ' ';
					break;
				case HatColor::WHITE:
					if (lastColor == -1 || lastColor != static_cast<int>(HatColor::BLUE))
						ss << "\033[47m";
					ss << ' ';
					break;
				case HatColor::RED:
					if (lastColor == -1 || lastColor != static_cast<int>(HatColor::BLUE))
						ss << "\033[41m";
					ss << ' ';
					break;
			}
		}
		ss << "\033[0m" << std::endl;
	}
	std::cout << ss.str();
}
