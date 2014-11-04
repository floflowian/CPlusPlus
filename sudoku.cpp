#include "stdafx.h"

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <boost/chrono/thread_clock.hpp>
#include <direct.h>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "sudoku.h"

using namespace std;
using namespace boost;
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

int Sudoku::multi(){
	boost::asio::io_service ioService;
	boost::thread_group threadpool;

	boost::asio::io_service::work work(ioService);
	for (int i = 0; i < 2; ++i){
		threadpool.create_thread(
			boost::bind(&boost::asio::io_service::run, &ioService)
			);
	}
	boost::asio::io_service * ptrStack = &ioService;
	ioService.post(boost::bind(bruteForce, 0,0,1,initialGrid,ptrStack));

	ioService.stop();
	threadpool.join_all();
	return 0;

}
int Sudoku::bruteForce(int i, int j, int value, int gridBeta[][9], boost::asio::io_service * ptrStack){
	//thread_clock::time_point start = thread_clock::now();
	boost::asio::io_service & currentStack = *ptrStack;
	vector<int> possibleValues;
	while (1){
		possibleValues.clear();
		for (int k = value; k <= 9; ++k){
			if (checkColumn(j, k, gridBeta) == 0 && checkRow(i, k, gridBeta) == 0 && checkSquare(i, j, k, gridBeta) == 0){
				possibleValues.insert(possibleValues.end(), k);
			}
		}
		if (possibleValues.size() == 0) return 0;
		else {
			if (possibleValues.size() > 1){
				for (int m = 1; m < possibleValues.size(); ++m){
					currentStack.post(boost::bind(bruteForce, i, j, possibleValues[m], gridBeta, ptrStack));
				}
			}
			gridBeta[i][j] = possibleValues[0];
		keepAdvancing:
			if (j == 8 && i == 8) { break; }
			if (j == 8){ ++i; j = 0; }
			else ++j;
			if (initialGrid[i][j] != 0) goto keepAdvancing;
			value = 1;
		}

	}
	//thread_clock::time_point stop = thread_clock::now();
	//cout << "Duration: " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl<<endl;
	printGrid();
	currentStack.stop();
	return 0;
}

int Sudoku::checkRow(int i, int value, int gridBeta[][9]){
	int *ptr = &gridBeta[i][0];
	if (*ptr == value) return -1;
	for (i = 1; i < 9; ++i)
	{
		++ptr;
		if (*ptr == value) return -1; 
	}
	return 0; 
}

int Sudoku::checkColumn(int i, int value, int gridBeta[][9]){
	int *ptr = &gridBeta[0][i];
	if (*ptr == value) return -1; 
	for (i = 1; i < 9; ++i)
	{
		ptr+=9;
		if (*ptr == value) return -1;
	}
	return 0; 
}

int Sudoku::checkSquare(int i, int j, int value, int gridBeta[][9]){
	int squareI = floor(i / 3);
	int squareJ = floor(j / 3);
	int *ptr = &gridBeta[squareI * 3][squareJ * 3];
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
