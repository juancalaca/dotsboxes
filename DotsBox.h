#ifndef DOTSBOX_H
#define DOTSBOX_H
#include <iostream>
#include "DotsEdge.h"

using namespace std;

class DotsBox
{
	DotsEdge *left;
	DotsEdge *top;
	DotsEdge *right;
	DotsEdge *bottom;
	
	bool claimed;
	string name;
	
public:

	DotsBox();
	
	~DotsBox();
	
	void SetLeft(DotsEdge *edge);
	
	void SetTop(DotsEdge *edge);
	
	void SetRight(DotsEdge *edge);
	
	void SetBottom(DotsEdge *edge);
	
	DotsEdge *GetLeft();
	
	DotsEdge *GetTop();
	
	DotsEdge *GetRight();
	
	DotsEdge *GetBottom();
	
	DotsEdge *GetEdge(int edge);
	
	void Claim(string name);
	
	bool NewlyClaimed();
	
	string GetName();
	
};
#endif
