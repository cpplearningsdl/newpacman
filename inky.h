#pragma once
#include "map.h"
#include "player.h"
#include "blinky.h"
#include "Astar.h"
#include <windows.h> 
#include <iostream>
#include "constants.h"


class inky :
	public blinky
{
public:
	inky(map &mymap);
	 
	void settarget(map &mymap, player &newplayer, blinky &newblinky);
	Astar::pos calctarget(player &newplayer, blinky &newblinky);   
};

