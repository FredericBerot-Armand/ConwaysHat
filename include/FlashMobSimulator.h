
#pragma once // FlashMobSimulator.h
#include <cstddef>
#include <chrono>
#include <vector>

/**
 * All participants will be arranged as a grid (8 neighbours around).
 * Each participant carries a set of colored hats ordered in a list (same for everyone), and wears only one.
 * Before the event starts, they choose randomly a hat to wear.
 * A rooster is used to orchestrate the choregraphy.
 *
 * Uppon a the rooster screams, participants will look around them and simultaneously (not one at a time) apply the following rule:
 * Each participant counts the number of neighbours who wears their next colored hat in the list.
 * If this number is greater than the threshold, then he/she will wear its next colored hat.
 * Else the participant doesn't switch hat.
 */
class FlashMobSimulator
{
public:
	enum HatColor
	{
		BLACK = 0,
		RED = 1,
		GREEN = 2,
		YELLOW = 3,
		BLUE = 4,
		PURPLE = 5,
		CYAN = 6,
		WHITE = 7,
		NONE
	};

	FlashMobSimulator(int width, int height, std::vector<HatColor> &colors);
	FlashMobSimulator(const FlashMobSimulator& flashMobSimulator);
	virtual ~FlashMobSimulator();
	FlashMobSimulator& operator=(const FlashMobSimulator& flashMobSimulator);

	int getWidth() const;
	int getHeight() const;
	int getHat(int x, int y) const;

	/**
	 * Run the simulation
	 * @param delay the delay between 2 rooster screams (in milliseconds)
	 */
	void run(int threshold, std::chrono::milliseconds delay, bool torus);

protected:
	int width_ = -1;
	int height_ = -1;
	std::vector<HatColor> colors_;
	int **grid_ = NULL;
	int **nextState_ = NULL;
	bool firstPrint_ = true;

	/**
	 * Free memory use by grid
	 */
	void reset();

	/**
	 * createGrid Create grid at the good size (width * height)
	 */
	void createGrid();

	/**
	 * Count the neighbors who have a @hatColor hat of the participant
	 * @param x Abscissa of the participant
	 * @param y Ordinate of the participant
	 * @param hatColor the index of the color of hat who count
	 * @return the number of neighbors who have a @hatColor hat
	 */
	int countNeighborsHats(int x, int y, int hatColor);

	/**
	 * Count the neighbors who have a @hatColor hat of the participant
	 * @param x Abscissa of the participant
	 * @param y Ordinate of the participant
	 * @param hatColor the index of the color of hat who count
	 * @return the number of neighbors who have a @hatColor hat
	 */
	int countNeighborsHatsOnTorus(int x, int y, int hatColor);

	/**
	 * Compute the next color of each hat.
	 * @return the number of participant who have change hat
	 */
	int computeNextState(int (*FlashMobSimulator::countFun)(int, int, int), int threshold);

	void printGrid();
};
