#include "DotsBox.h"
#include <iostream>

using namespace std;

// DotsBox constructor
DotsBox::DotsBox()
{
	this->left = NULL;
	this->top = NULL;
	this->right = NULL;
	this->bottom = NULL;
	
	this->claimed = false;
	this->name = "  ";
}

// class destructor
// can't truly delete edges due to circular references
DotsBox::~DotsBox() { 
	this->left = NULL;
	this->right = NULL;
	this->top = NULL;
	this->bottom = NULL;
}

// setter for left edge
void DotsBox::SetLeft(DotsEdge *edge)
{
	this->left = edge;
}

// setter for top edge
void DotsBox::SetTop(DotsEdge *edge)
{
	this->top = edge;
}

// setter for right edge
void DotsBox::SetRight(DotsEdge *edge)
{
	this->right = edge;
}

// setter for bottom edge
void DotsBox::SetBottom(DotsEdge *edge)
{
	this->bottom = edge;
}

// getter for left edge
DotsEdge *DotsBox::GetLeft()
{
	return this->left;
}

// getter for top edge
DotsEdge *DotsBox::GetTop()
{
	return this->top;
}

// getter for right edge
DotsEdge *DotsBox::GetRight()
{
	return this->right;
}

// getter for bottom edge
DotsEdge *DotsBox::GetBottom()
{
	return this->bottom;
}

// returns a pointer to an edge based on the given number
// (1 = left, 2 = top, 3 = right, 4 = bottom, anything else is NULL)
DotsEdge *DotsBox::GetEdge(int edge_num)
{
	DotsEdge *edge;
	switch (edge_num)
	{
		case 1:
			edge = this->left;
			break;
		case 2:
			edge = this->top;
			break;
		case 3:
			edge = this->right;
			break;
		case 4:
			edge = this->bottom;
			break;
		default:
			edge = NULL;
	}
	
	return edge;
}

// sets the claimed flag to true
// sets the "name" member to the given string
void DotsBox::Claim(string name)
{
	this->claimed = true;
	this->name = name;
}

// returns the name class member
string DotsBox::GetName()
{
	return this->name;
}

// determines if the box was just claimed
// by checking the status of all its edges
// and then checking that its "claimed" flag
// had already been set to true
bool DotsBox::NewlyClaimed()
{
	return (!this->claimed && left->IsClaimed() && right->IsClaimed() && top->IsClaimed() && bottom->IsClaimed());
}
