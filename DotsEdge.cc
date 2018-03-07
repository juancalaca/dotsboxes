#include "DotsEdge.h"
#include "DotsBox.h"
#include <string>

using namespace std;

DotsEdge::DotsEdge()
{
	this->box1 = NULL;
	this->box2 = NULL;
	
	this->claimed = false;
}

DotsEdge::~DotsEdge() {}

void DotsEdge::SetBox1(DotsBox *box)
{
	this->box1 = box;
}

void DotsEdge::SetBox2(DotsBox *box)
{
	this->box2 = box;
}

bool DotsEdge::IsClaimed()
{
	return this->claimed;
}

int DotsEdge::Claim(string name)
{
	int claimedBoxes = 0;
	
	this->claimed = true;
	
	if (this->box1->NewlyClaimed())
	{
		cout << name << " claimed a box" << endl;
		this->box1->Claim(name);
		claimedBoxes++;
	}
	
	if (this->box2 != NULL && this->box2->NewlyClaimed())
	{
		cout << name << " claimed a box" << endl;
		this->box2->Claim(name);
		claimedBoxes++;
	}
	
	return claimedBoxes;
}
