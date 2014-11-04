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
	static int multi();
 	 int bruteForce(int i, int j, int value, int grid[][9], boost::asio::io_service * ptrStack);
	 int checkRow(int i, int value, int gridBeta[][9]);
	 int checkColumn(int i, int value, int gridBeta[][9]);
	 int checkSquare(int i, int j, int value, int gridBeta[][9]);

};

