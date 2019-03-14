#include <stdafx.h>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <time.h>

#include "Grid.h"

using namespace std;

Grid::Grid()
{
	random_device randomDevice;
	mt19937 gen(randomDevice());
	uniform_int_distribution <> dist(0, species);

	updateValues();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			// 0 represents a dead cell
			currentGrid[i][j] = (int)dist(gen);
		}
	}
}

int Grid::getWidth()
{
	return width;
}

int Grid::getHeight()
{
	return height;
}

int Grid::checkCell(int x, int y)
{
	return currentGrid[x][y];
}

void Grid::changeCell(int x, int y, int species)
{
	currentGrid[x][y] = species;
}

int Grid::checkNextGenCell(int x, int y)
{
	return nextGenGrid[x][y];
}

void Grid::changeNextGenCell(int x, int y, int species)
{
	nextGenGrid[x][y] = species;
}

int Grid::numOfNeighbours(int x, int y)
{
	int numOfNeightbours = 0;

	// Check conditions for specific species
	if (checkCell(x, y) != 0)
	{
		int xStart = max(x - 1, 0);
		int xFinish = min(x + 1, width - 1);
		int yStart = max(y - 1, 0);
		int yFinish = min(y + 1, height - 1);

		for (int currentY = xStart; currentY <= xFinish; currentY++)
		{
			for (int currentX = yStart; currentX <= yFinish; currentX++)
			{
				// Check if the same species and ignore itself
				if (checkCell(x, y) == checkCell(currentY, currentX) && !(currentY == x && currentY == y))
				{
					numOfNeightbours++;
				}
			}
		}
	}

	return numOfNeightbours;
}

// Will look for three neighbours of the same species to ressurect a dead cell
// Return the species that has three neighbours to the cell OR return 0 if conditions not met
int Grid::reproduce(int x, int y)
{
	int xStart = max(x - 1, 0);
	int xFinish = min(x + 1, width - 1);
	int yStart = max(y - 1, 0);
	int yFinish = min(x + 1, height - 1);

	for (int i = 1; i <= species; i++)
	{
		int numberOfLivingNeighbours = 0;
		for (int currentY = xStart; currentY <= xFinish; currentY++)
		{
			for (int currentX = yStart; currentX <= yFinish; currentX++)
			{
				if (checkCell(currentY, currentX) != 0)
				{
					//Checks if the same species and ignores itself
					if (checkCell(currentY, currentX) != 0)
					{
						numberOfLivingNeighbours++;
					}
				}
			}
		}

		if (numberOfLivingNeighbours == 3)
		{
			return i;
		}
	}
	return 0;
}

// Call numOfNeighbours for living cells of reproduce for dead cells, then update the corresponding nextGenGrid cell
void Grid::nextGen(int x, int y)
{
	if (checkCell(x, y) != 0)
	{
		int neighbours = numOfNeighbours(x, y);
		
		if (neighbours < 2 || neighbours > 3)
		{
			changeNextGenCell(x, y, 0);
		}
		else
		{
			changeNextGenCell(x, y, checkCell(x, y));
		}
	}
	else
	{
		changeNextGenCell(x, y, reproduce(x, y));
	}
}

// Update the m * n cells of the grid to the next generation
void Grid::update(int xStart, int xFinish)
{
	// Populate the nextGenGrid with results
	for (int i = xStart; i < xFinish; i++)
	{
		for (int j = 0; j < height; j++)
		{
			nextGen(i, j);
		}
	}
}

// Transition the currentGrid to the nextGenGrid
void Grid::transition(int xStart, int xFinish)
{
	for (int i = xStart; i < xFinish; i++)
	{
		for (int j = 0; j < height; j++)
		{
			changeCell(i, j, checkNextGenCell(i, j));
		}
	}
}

void Grid::updateValues()
{
	int tempValue;
	cout << "Enter the values for width x height (maximum 1000): " << endl;
	cin >> tempValue;
	width = tempValue;
	height = tempValue;
}