#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "sudoku.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Sudoku sudoku;
	sudoku.printGrid();
	sudoku.bruteForce();
	cin.sync();
	cin.get();
	return 0;
}


