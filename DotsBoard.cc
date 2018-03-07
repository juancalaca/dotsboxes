#include "DotsBoard.h"
#include "DotsEdge.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

DotsBoard::DotsBoard(int rows, int columns)
{
	this->rows = rows;
	this->columns = columns;
	this->claimedBoxes = 0;
	this->totalBoxes = (rows * columns);
	
	for (int r = 0; r < rows; r++)
	{
		vector<DotsBox*> newRow;
		
		for (int c = 0; c < columns; c++)
		{
			DotsBox *box = new DotsBox();
			
			DotsEdge *bottom = new DotsEdge();
			bottom->SetBox1(box);
			
			DotsEdge *right = new DotsEdge();
			right->SetBox1(box);

			DotsEdge *left;
			if (c == 0)
			{
				left = new DotsEdge();
				left->SetBox1(box);
			}
			else
			{
				left = newRow[(c - 1)]->GetRight();
				left->SetBox2(box);
			}
			
			DotsEdge *top;
			if (r == 0)
			{
				top = new DotsEdge();
				top->SetBox1(box);
			}
			else 
			{
				top = this->boxes[(r - 1)][c]->GetBottom();
				top->SetBox2(box);
			}
			
			box->SetLeft(left);
			box->SetRight(right);
			box->SetTop(top);
			box->SetBottom(bottom);
			
			newRow.push_back(box);
		}
		
		this->boxes.push_back(newRow);
	}
}

DotsBoard::~DotsBoard()
{
	//Erase vector elements
	boxes.erase(boxes.begin(), boxes.begin() + totalBoxes);
}


int DotsBoard::ClaimEdge(int row, int column, int edge_num, string name)
{
	DotsEdge *edge = this->boxes[row][column]->GetEdge(edge_num);
	
	//if that edge is taken return -1
	if (edge->IsClaimed())
	{
		return -1;
	}
	
	int new_claimed =  edge->Claim(name);
	this->claimedBoxes += new_claimed;
	return new_claimed;

}

bool DotsBoard::GameOver()
{
	return (this->claimedBoxes == this->totalBoxes);
}

void DotsBoard::PrintBoard()
{
	this->PrintSelect(-1, -1);
}

void DotsBoard::PrintSelect(int selected_row, int selected_column)
{
	this->PrintSelectEdge(selected_row, selected_column, 0);
}

void DotsBoard::PrintSelectEdge(int selected_row, int selected_column, int edge)
{
	//print out a line to separate the board from previous prints
	cout << endl << endl;
	cout << "==================================================================" << endl;
	cout << endl << endl;

	//determine which selection indicator to use
	string symbol;
	switch (edge)
	{
		case 1:
			symbol = "<<";
			break;
		case 2:
			symbol = "/\\";
			break;
		case 3:
			symbol = ">>";
			break;
		case 4:
			symbol = "\\/";
			break;
		default:
			symbol = "##";
			break;
	}
	
	//create the string-representation of the board and print it
	for (int r = 0; r < this->rows; r++)
	{
		string top_string = "*";
		string other_lines = "";
		string names_line = "";
		
		for (int c = 0; c < this->columns; c++)
		{
			if (this->boxes[r][c]->GetTop()->IsClaimed())
			{
				top_string.append("------*");
			}
			else 
			{
				top_string.append("      *");
			}
			
			if (this->boxes[r][c]->GetLeft()->IsClaimed())
			{
				other_lines.append("|");
				names_line.append("|");
			}
			else
			{
				other_lines.append(" ");
				names_line.append(" ");
			}
			
			names_line.append("  ");
			//this will always be two chars!
			names_line.append(boxes[r][c]->GetName());
			
			names_line.append("  ");			

			if (c == selected_column && r == selected_row)
			{
				other_lines.append("  ");
				other_lines.append(symbol);
				other_lines.append("  ");
			}
			else 
			{
				other_lines.append("      ");
			}		
			
			if ( c == (columns - 1))
			{
				if (this->boxes[r][c]->GetRight()->IsClaimed())
				{
					other_lines.append("|");
					names_line.append("|");
				}
				else
				{
					other_lines.append(" ");
					names_line.append(" ");
				}
			}
		}
		
		cout << top_string << endl;
		cout << other_lines << endl;
		cout << names_line << endl;
		cout << other_lines << endl;
	}
	
	string bottom_string = "*";
	
	for (int c = 0; c < this->columns; c++)
	{
		if (this->boxes[rows-1][c]->GetBottom()->IsClaimed())
		{
			bottom_string.append("------*");
		}
		else 
		{
			bottom_string.append("      *");
		}
	}
	
	cout << bottom_string << endl;
}
