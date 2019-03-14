#include <stdafx.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <GL/freeglut.h>
#include "Grid.h"

using namespace std;

//static const int numOfSpecies = 1;
static const int numOfThreads = 1;

Grid grid = Grid();
int threadWidth = grid.getWidth() / numOfThreads;

void nextGrid(int tid)
{
	grid.update(threadWidth * tid, threadWidth * (tid + 1) - 1);
}

void transitionGrid(int tid)
{
	grid.transition(threadWidth * tid, threadWidth * (tid + 1) - 1);
}

void refreshScreen(void)
{
	thread t[numOfThreads];

	// Populate the nextGen with threads, split the task
	for (int i = 0; i < numOfThreads; i++)
	{
		t[i] = thread(nextGrid, i);
	}

	// Join all the threads back
	for (int i = 0; i < numOfThreads; i++)
	{
		t[i].join();
		if (i == numOfThreads - 1) {}
	}

	// Transition the nextGenGrid to the currentGrid
	for (int i = 0; i < numOfThreads; i++)
	{
		t[i] = thread(transitionGrid, i);
	}

	// Join all the threads back
	for (int i = 0; i < numOfThreads; i++)
	{
		t[i].join();
	}

	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0.0f, grid.getWidth(), grid.getHeight(), 0.0f, 0.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(3.0f);
	glBegin(GL_POINTS);

	for (int i = 0; i < grid.getWidth(); i++)
	{
		for (int j = 0; j < grid.getHeight(); j++)
		{
			switch (grid.checkCell(i, j))
			{
			case 0:
				// Black
				glColor3f(0.0f, 0.0f, 0.0f);
				break;
			case 1:
				// Red
				glColor3f(0.42f, 0.557f, 0.137f);
				break;
			}
			glVertex2f(i, j);
		}
	}

	glEnd();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0.0, 0.0);
	glutInitWindowSize(grid.getWidth() * 4, grid.getHeight() * 4);
	glutCreateWindow("The Game Of Life");
	glutDisplayFunc(display);
	glutIdleFunc(refreshScreen);
	glutMainLoop();
	return 0;
}