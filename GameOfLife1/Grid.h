#pragma once
#pragma once

class Grid
{
private:
	int species = 1;
	int width;
	int height;
	int currentGrid[1000][1000];
	int nextGenGrid[1000][1000];
	
public:
	Grid();
	int getWidth();
	int getHeight();
	int checkCell(int x, int y);
	int checkNextGenCell(int x, int y);
	void changeCell(int x, int y, int species);
	void changeNextGenCell(int x, int y, int species);
	int numOfNeighbours(int x, int y);
	int reproduce(int x, int y);
	void nextGen(int x, int y);
	void update(int xStart, int xFinish);
	void transition(int xStart, int xFinish);
	void updateValues();
};