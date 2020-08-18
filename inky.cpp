#include "inky.h" 

inky::inky(map &mymap)
{
	inky::x = mymap.inkypos.x;
	inky::y = mymap.inkypos.y;
	inky::dir = UP;
	inky::defaultx = mymap.inkypos.x;
	inky::defaulty = mymap.inkypos.y;
	inky::defaultdir = UP;
	inky::color = 8;
	inky::homex = mymap.inkyhome.x;
	inky::homey = mymap.inkyhome.y;

	inky::targetx = mymap.houseexit.x;
	inky::targety = mymap.houseexit.y;

	inky::pelletcount = 3;
	inky::defaultpelletcount =3;

	inky::mood = WAIT;
	inky::alive = false;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	inky::draw();
} 
void inky::settarget(map &mymap, player &newplayer, blinky &newblinky)
{ 
	Astar::pos pos;

	switch (mood)
	{
	case CHASE:
		pos = calctarget(newplayer, newblinky);
		inky::targetx = pos.x;
		inky::targety = pos.y;
	case SCATTER:
		inky::targetx = inky::homex;
		inky::targety = inky::homey;
		break;
	case PANIC:
		Astar::pos pos = randomadjacent(mymap);
		inky::targetx = pos.x;
		inky::targety = pos.y;
		break;
	case LEAVE:
		inky::targetx = 13;
		inky::targety = 11;
		break;
	case EYES:
		inky::targetx = inky::defaultx;
		inky::targety = inky::defaulty;
		break;
	}
}

Astar::pos inky::calctarget(player &newplayer, blinky &newblinky)
{
	Astar::pos targetpos, pacpos;
	
	pacpos.x = newplayer.playerx; 
	pacpos.y = newplayer.playery; 

	//move target 2 places in front of pacman
	switch(newplayer.playerdirection)
	{
		case UP:
			pacpos.y = pacpos.y - 2; 
			break;
		case LEFT:
			pacpos.x = pacpos.x - 2;
			break;
		case DOWN:
			pacpos.y = pacpos.y + 2;
			break;
		case RIGHT:
			pacpos.x = pacpos.x + 2;
			break;
	}
	//move target pos to pacpos
	//inkys target is basically a line from blinky, to the adjusted pacpos, extending past that point in the same direction the same distance. so if blinky is SE of pacman, inkys target is NW of pacman adjusted pos
	targetpos = pacpos;

	//adjust targetpos x the same distance blinky is from pacpos, need to test with >= etc..
	if (newblinky.x > pacpos.x)
	{
		targetpos.x = targetpos.x - (newblinky.x - pacpos.x);
	}else
	if (newblinky.x < pacpos.x)
	{
		targetpos.x = targetpos.x + (pacpos.x - newblinky.x);
	}
	//adjust targetpos y the same distance blinky is from pacpos
	if (newblinky.y < pacpos.y)
	{
		targetpos.y = targetpos.y + (pacpos.y - newblinky.y);
	}else
	if (newblinky.y > pacpos.y)
	{
		targetpos.y = targetpos.y - (newblinky.y - pacpos.y);
	} 
	return targetpos;
}
 
