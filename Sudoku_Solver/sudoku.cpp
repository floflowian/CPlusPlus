#include "stdafx.h"
#include "sudoku.h"
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <boost/chrono/thread_clock.hpp>
#include <direct.h>

using namespace std;
using namespace boost::chrono;

Sudoku::Sudoku()
{
	char *path = NULL;
	path = _getcwd(NULL, 0);
	char number[10];
	cout << "Grid number : ";
	cin >> number;
	string gridfile;
	gridfile.insert(gridfile.size(), path);
	gridfile.insert(gridfile.size(), "/grids/grid");
	gridfile.insert(gridfile.size(), number);
	gridfile.insert(gridfile.size(), ".txt");

	ifstream streamGrid(gridfile);
	if (!streamGrid){ cout << "Unable to read grid file" << endl; return; }

	string lineRead;
	for (int i = 0; i < 9; ++i){
		getline(streamGrid, lineRead);
		for (int j = 0; j < 9; ++j){
			if (lineRead[j] == '.') { grid[i][j] = 0; initialGrid[i][j] = 0; }
			else { grid[i][j] = lineRead[j] - 48; initialGrid[i][j] = lineRead[j] - 48; }
		}
	}
}

Sudoku::~Sudoku()
{
}

void Sudoku::printGrid(){
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < 9; ++j){
			cout << grid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Sudoku::bruteForce(){
	thread_clock::time_point start = thread_clock::now();
	int value = 1;
	int i = 0, j = 0;
	while (1){
		if (checkColumn(j, value) == 0 && checkRow(i, value) == 0 && checkSquare(i, j, value) == 0){ //Next cell
			grid[i][j] = value;
			keepAdvancing:
				if (j == 8 && i == 8) { break; }
				if (j == 8){ ++i; j = 0; }
				else ++j;
				if (initialGrid[i][j] != 0) goto keepAdvancing;
				value = 1;
		}

		else if (value == 9){ //Backtrack
			keepBacktracking:
			if (j != 0) --j; 
			else{--i; j = 8;}
			if (initialGrid[i][j] != 0) goto keepBacktracking;
			if (grid[i][j] != 9) {value = grid[i][j] + 1; grid[i][j] = 0;}
			else {
					grid[i][j] = 0; goto keepBacktracking;
				}
		}

		else ++value; //Increment current cell
	}
	thread_clock::time_point stop = thread_clock::now();
	cout << "Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl<<endl;
	printGrid();
}

int Sudoku::checkRow(int i, int value){
	int *ptr = &grid[i][0];
	if (*ptr == value) return -1;
	for (i = 1; i < 9; ++i)
	{
		++ptr;
		if (*ptr == value) return -1; 
	}
	return 0; 
}

int Sudoku::checkColumn(int i, int value){
	int *ptr = &grid[0][i];
	if (*ptr == value) return -1; 
	for (i = 1; i < 9; ++i)
	{
		ptr+=9;
		if (*ptr == value) return -1;
	}
	return 0; 
}

int Sudoku::checkSquare(int i,int j, int value){
	int squareI = floor(i / 3);
	int squareJ = floor(j / 3);
	int *ptr = &grid[squareI*3][squareJ*3];
	if (*ptr == value) return -1; 
	for (i = 1; i <=3; ++i)
	{
		++ptr;
		if (*ptr == value)  return -1; 
		++ptr;
		if (*ptr == value) return -1; 
		if (i != 3){
			ptr += 7;
			if (*ptr == value) return -1;
		}
	}
	return 0;
}