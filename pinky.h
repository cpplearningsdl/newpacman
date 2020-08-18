#pragma once
#include "blinky.h"
#include "constants.h"
 
class pinky :
	public blinky
{
public: 
	pinky(map &mymap);
	void settarget(map &mymap, player &newplayer);
};

