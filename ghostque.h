#pragma once
#include "blinky.h" 

class ghostque
{
public:
	ghostque();
	~ghostque();
	 
	int curghost;

	void atepellet(blinky &newblinky); 
};

