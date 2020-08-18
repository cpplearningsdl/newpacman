#include "clyde.h"
 
clyde::clyde(map &mymap)
{
	clyde::x = mymap.clydepos.x;
	clyde::y = mymap.clydepos.y;
	clyde::dir = LEFT;
	clyde::defaultx = mymap.clydepos.x;
	clyde::defaulty = mymap.clydepos.y;
	clyde::defaultdir = UP;
	clyde::color = 6;
	clyde::homex = mymap.clydehome.x;
	clyde::homey = mymap.clydehome.y;

	clyde::targetx = mymap.houseexit.x;
	clyde::targety = mymap.houseexit.y;

	clyde::pelletcount = 5;
	clyde::defaultpelletcount = 11;

	clyde::mood = WAIT;
	clyde::alive = false;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	clyde::draw();
}
 

void clyde::settarget(map &mymap, player &newplayer)
{	
	int dist;

	switch (mood)
	{
	case CHASE:
		//clyde uses pacs xy if he is far away, but if he gets close he chickens out and goes home
		dist = distance(clyde::x, clyde::y, newplayer.playerx, newplayer.playery);
		if(dist > 7)
		{
			clyde::targetx = newplayer.playerx;
			clyde::targety = newplayer.playery;
			break;
		}else
			{
			clyde::targetx = clyde::homex;
			clyde::targety = clyde::homey;
			break;
			}
		break;
	case SCATTER:
		clyde::targetx = clyde::homex;
		clyde::targety = clyde::homey;
		break;
	case PANIC:
		Astar::pos pos = randomadjacent(mymap);
		clyde::targetx = pos.x;
		clyde::targety = pos.y;
		break;
	case LEAVE:
		clyde::targetx = 13;
		clyde::targety = 11;
		break;
	case EYES:
		clyde::targetx = clyde::defaultx;
		clyde::targety = clyde::defaulty;
		break;
	}
}
 
