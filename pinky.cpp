#include "pinky.h"

 
pinky::pinky(map &mymap)
{
	//current xy 
	pinky::x = mymap.pinkypos.x;
	pinky::y = mymap.pinkypos.y;
	pinky::dir = UP;
	//ghosthouse xy
	pinky::defaultx = mymap.pinkypos.x;
	pinky::defaulty = mymap.pinkypos.y;
	pinky::defaultdir = UP;
	pinky::color = 13;
	//scatter target
	pinky::homex = mymap.pinkyhome.x;
	pinky::homey = mymap.pinkyhome.y;
	//current target xy
	pinky::targetx = mymap.houseexit.x;
	pinky::targety = mymap.houseexit.y;
	//pellet count for que
	pinky::pelletcount = 3;
	pinky::defaultpelletcount = 2;

	pinky::mood = WAIT;
	pinky::alive = false;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	pinky::draw();
}
 
void pinky::settarget(map &mymap, player &newplayer)
{
	gotoxy(31, 24); std::cout << "ffffffffff";
	switch (pinky::mood)
	{
	case CHASE:
		switch (newplayer.playerdirection)
		{
		case UP:
			pinky::targetx = newplayer.playerx;
			pinky::targety = newplayer.playery - 4;
			break;
		case LEFT:
			pinky::targetx = newplayer.playerx - 4;
			pinky::targety = newplayer.playery;
			break;
		case DOWN:
			pinky::targetx = newplayer.playerx;
			pinky::targety = newplayer.playery + 4;
			break;
		case RIGHT:
			pinky::targetx = newplayer.playerx + 4;
			pinky::targety = newplayer.playery;
			break;
		}
		break;

	case SCATTER:
		pinky::targetx = mymap.pinkyhome.x;
		pinky::targety = mymap.pinkyhome.y;
		break;
	case PANIC:
		Astar::pos pos = randomadjacent(mymap);
		pinky::targetx = pos.x;
		pinky::targety = pos.y;
		break;
	case LEAVE:
		pinky::targetx = mymap.houseexit.x;
		pinky::targety = mymap.houseexit.y;
		break;
	case EYES:
		pinky::targetx = pinky::defaultx;
		pinky::targety = pinky::defaulty;
		break;
	}
}
