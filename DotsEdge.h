#ifndef DOTSEDGE_H
#define DOTSEDGE_H

#include <iostream>

//forward reference
class DotsBox;

class DotsEdge
{
	DotsBox *box1;
	DotsBox *box2;
	
	bool claimed;
	
public:
	DotsEdge();
	
	~DotsEdge();
	
	void SetBox1(DotsBox *box);
	
	void SetBox2(DotsBox *box2);
	
	bool IsClaimed();
	
	int Claim(std::string name);
};

#endif