#pragma once
#include "blinky.h" 
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
#include "map.h"
#include "constants.h"

class que
{
public:

	que();
	~que();
	
	int time, scatter;

	void doque(blinky &newblinky, pinky &newpinky, inky &newinky, clyde &newclyde, map &mymap);
	int nextghost(bool blinky, bool pinky, bool inky, bool clyde); 

};

