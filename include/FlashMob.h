
#pragma once // FlashMob.h
#include <cstddef>
#include <chrono>

class FlashMob
{
public:
	enum HatColor
	{
		BLUE = 0,
		WHITE = 1,
		RED = 2
	};
	FlashMob(int width, int height);
	FlashMob(const FlashMob& flashMob);
	virtual ~FlashMob();
	FlashMob& operator=(const FlashMob& flashMob);
	
	int getWidth() const;
	int getHeight() const;
	HatColor& getHat(int x, int y) const;

	void run(std::chrono::milliseconds delay);

protected:
	int width = -1;
	int height = -1;
	HatColor **grid = NULL;
	HatColor **nextState = NULL;

	bool firstPrint = true;

	void reset();
	void createGrid();
	int countNeighborsHats(int x, int y, HatColor hatColor);
	int computeNextState();
	virtual void printGrid();
};