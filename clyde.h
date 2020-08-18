#pragma once
#include "constants.h"
#include "blinky.h"
 
class clyde :
	public blinky
{
public:
	clyde(map &mymap); 
	void settarget(map &mymap, player &newplayer);  
};

