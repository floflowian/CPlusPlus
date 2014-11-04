#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "sudoku.h"


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Sudoku sudoku;
	sudoku.printGrid();
	sudoku.multi();
	cin.sync();
	cin.get();
	return 0;
}


