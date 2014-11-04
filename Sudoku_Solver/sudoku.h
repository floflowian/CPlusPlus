#pragma once
class Sudoku
{
private:
	int grid[9][9];
	int initialGrid[9][9];
public:
	Sudoku();
	~Sudoku();
	void printGrid();
	void bruteForce();
	int checkCell(int i, int j, int value);
	int checkRow(int i, int value);
	int checkColumn(int i, int value);
	int checkSquare(int i, int j, int value);

};

