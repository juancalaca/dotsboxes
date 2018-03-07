#ifndef DOTSBOARD_H
#define DOTSBOARD_H

#include <vector>
#include "DotsBox.h"

class DotsBoard
{
	// Class variables
	int rows;
	int columns;
	int claimedBoxes;
	int totalBoxes;
	std::vector<std::vector< DotsBox* > > boxes;
	
public:
	DotsBoard(int rows, int columns);
	
	~DotsBoard();
	
	int ClaimEdge(int row, int column, int edge, string name);
	
	bool GameOver();
	
	void PrintBoard();
	
	void PrintSelect(int selected_row, int selected_column);
	
	void PrintSelectEdge(int selected_row, int selected_column, int edge);
};
#endif