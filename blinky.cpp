#include "blinky.h"  

blinky::blinky(map &mymap)
	{
		blinky::x = mymap.blinkypos.x;
		blinky::y = mymap.blinkypos.y;
		blinky::dir = LEFT;
		blinky::defaultx = blinky::x;
		blinky::defaulty = blinky::y;
		blinky::defaultdir = LEFT;
		blinky::color = 12;

		blinky::homex = mymap.blinkyhome.x;
		blinky::homey = mymap.blinkyhome.y;
		 
		blinky::targetx = mymap.houseexit.x;
		blinky::targety = mymap.houseexit.y;
		  
		blinky::pelletcount = 1;
		blinky::defaultpelletcount = 1;

		blinky::mood = WAIT;
		blinky::alive = false;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		blinky::draw();
	}
 
blinky::blinky()
{ 
}

int blinky::distance(int x1, int y1, int x2, int y2)
{
	int distance = (abs(x1 - x2) + abs(y1 - y2));
	return distance;
}
  
void blinky::move2(map &mymap, player &newplayer, blinky &newblinky)
{ 
	if (newplayer.alive)
	{
		if (blinky::alive)
		{
			//handle on/off of panic modethis should be inside the lower if..
			blinky::paniccheck(newplayer);

			//if eaten by pacman, die. this should be inside the lower if..
			if (blinky::mood == PANIC && blinky::x == newplayer.playerx && blinky::y == newplayer.playery)
			{
				blinky::die();
				newplayer.score = newplayer.score + 150; 
			} 

			int oldx, oldy;
			oldx = blinky::x; oldy = blinky::y;
			Astar::pos nextxy;
			int tile = mymap.themap[y][x];

			//teleport
			switch (tile)
			{
			case 8:
				blinky::erase();
				blinky::x = mymap.warp2.x;
				blinky::y = mymap.warp2.y;
				oldx = blinky::x;
				oldy = blinky::y;
				draw();
				break;
			case 9:
				erase();
				blinky::x = mymap.warp.x;
				blinky::y = mymap.warp.y;
				oldx = blinky::x;
				oldy = blinky::y;
				draw();
				break;
			}
 
			//set target square
			blinky::settarget(mymap, newplayer);

			//check if can eat pac 
			if (!blinky::eatpac(newplayer))
			{ 
					nextxy = Astar::Pathfinding(mymap, x, y, targetx, targety, dir, mood);

				//stuck at destination or blocked path(which shouldnt be possible in pacman but whateveah)
				if ( oldx == nextxy.x && oldy == nextxy.y || 0 == nextxy.x && 0 == nextxy.y)
				{ 
					nextxy = randomadjacent(mymap);
				} 

				//update direction, should have designed a* and random adjacent to return direction not pos and could have avoided this! 
					if (nextxy.y == blinky::y - 1) 
						{ blinky::dir = UP; }
					else 
					if (nextxy.x == blinky::x - 1) 
						{ blinky::dir = LEFT; }
					else
					if (nextxy.y == blinky::y + 1)
						{ blinky::dir = DOWN; }
					else
					if (nextxy.x == blinky::x + 1) 
						{ blinky::dir = RIGHT; } 
				//actually update xy position
				blinky::x = nextxy.x;
				blinky::y = nextxy.y; 
			} 

			//check if any special goals are met, some moods are trying to do something specific, handle here 
			switch (blinky::mood)
			{
				//panic chooses random direction at each intersection, set target handled this for us so now just update ghost position variables
				//also eatpac returns false if in panic mood so thats why it wasnt in that if() up there.
				case PANIC:
				{
					blinky::x = targetx;
					blinky::y = targety;
					//if eaten by pacman, die, this block of code again here in case randomadjacented onto pacman?
					if (blinky::x == newplayer.playerx && blinky::y == newplayer.playery)
					{
						blinky::die();
						newplayer.score = newplayer.score + 150;
					}
					break;
				}
				case EYES:
				{	//returned to ghost house success, technically the eyes are 'alive' until they get back 
					if (blinky::x == blinky::defaultx && blinky::y == blinky::defaulty)
						{
							blinky::mood = WAIT;
							blinky::alive = false; 
						}
					break;
				}
				case LEAVE:
				{	//leave ghost house success, flip mood to chase, could probably get rid of this with modifications to testwalkable() to detect direction ghost facing tiles
					if (blinky::x == blinky::targetx && blinky::y == blinky::targety)
					{
						blinky::mood = CHASE; 
						blinky::alive = true;
					}
					break;
				} 
			}  
			//draw whatever ghost stepped on and ghost
			mymap.rebuild(oldx, oldy);
			blinky::draw(); 
		}
	}
	else//pac was dead
	{
		blinky::reset();
	} 
}

void blinky::scatter()
{
	if(blinky::alive && blinky::mood == SCATTER)
	{
		blinky::mood = CHASE;
	}else if(blinky::alive && blinky::mood == CHASE)
	{ 
		blinky::mood = SCATTER;
	}
}
  
void blinky::settarget(map &mymap, player &newplayer)
{   
		switch (mood)
		{
		case CHASE:
			blinky::targetx = newplayer.playerx;
			blinky::targety = newplayer.playery;
			break;
		case SCATTER:  
			blinky::targetx = blinky::homex;
			blinky::targety = blinky::homey;
			break;
		case PANIC: 
			Astar::pos pos = blinky::randomadjacent(mymap);
			blinky::targetx = pos.x;
			blinky::targety = pos.y;
			break;
		case LEAVE:
			blinky::targetx = mymap.houseexit.x;
			blinky::targety = mymap.houseexit.y;
			break;
		case EYES:
			blinky::targetx = blinky::defaultx;
			blinky::targety = blinky::defaulty;
			break;
		}  
}

Astar::pos blinky::randomadjacent(map &mymap)
{
	Astar::pos pos; pos.x = x; pos.y = y;
	int valid = 0;
	int validlist[3]; 
	int random;
	//up
	if (walkable(mymap, x, y-1) && dir != DOWN)
	{
		validlist[valid] = 0;
		valid = valid++;
	}
	//left
	if (walkable(mymap, x-1, y) && dir != RIGHT)
	{
		validlist[valid] = 1;
		valid = valid++;
	}
	//down
	if (walkable(mymap, x, y + 1) && dir != UP)
	{
		validlist[valid] = 2;
		valid = valid++;
	}
	//right
	if (walkable(mymap, x+1, y) && dir != LEFT)
	{
		validlist[valid] =  3;
		valid = valid++;
	}

	if(valid == 0)//incase dead end while panic mode
	{
		random = rand() % 3;
	}
	else {
		random = rand() % valid;  
		random = validlist[random];
	}

	switch (random)
	{
	case 0: 
		pos.y--;
		dir = UP;
		break;
	case 1: 
		pos.x--;
		dir = LEFT;
		break;
	case 2:
		pos.y++;
		dir = DOWN;
		break;
	case 3: 
		pos.x++;
		dir = RIGHT;
	} 
	return pos;
}

bool blinky::eatpac(player &newplayer)
{
	//cant eat while panic pilld or eyes
	if(blinky::mood == PANIC || blinky::mood == EYES)
	{  
		return false; 
	}
	else
	{ 		   
		//pac moved onto us 
		if (newplayer.playerx == blinky::x && newplayer.playery == blinky::y)
		{
			newplayer.kill(); 
			return true;
		}
		else //up
		if (newplayer.playerx == blinky::x && newplayer.playery == blinky::y - 1 && blinky::dir != DOWN)
		{
			blinky::y = blinky::y--;
			newplayer.kill(); 
			return true;
		}
		else //left
		if (newplayer.playerx == blinky::x-1 && newplayer.playery == blinky::y && blinky::dir != RIGHT)
		{
			blinky::x = blinky::x--;
			newplayer.kill();
			return true;
		}
		else //down
			if (newplayer.playerx == blinky::x && newplayer.playery == blinky::y + 1 && blinky::dir != UP)
		{ 
			blinky::y = blinky::y++;
			newplayer.kill();
			return true;
		}
		else //right
			if (newplayer.playerx == blinky::x + 1 && newplayer.playery == blinky::y && blinky::dir != LEFT)
		{ 
			blinky::x = blinky::x++;
			newplayer.kill();
			return true;
		} 

		//not eatable, so false
		return false;
	}
}

void blinky::reset()
{
	if(blinky::alive)
	{
		blinky::erase();
		blinky::x = blinky::defaultx;
		blinky::y = blinky::defaulty;
		blinky::dir = blinky::defaultdir;
		blinky::alive = false;
		blinky::mood = WAIT;
		blinky::draw();
	}
}

void blinky::die()
{
	blinky::erase(); 
	blinky::mood = EYES; 
	blinky::draw(); 
}
	 
bool blinky::walkable(map &mymap, int tx, int ty)
	{ 
		if (mymap.themap[ty][tx] != 1 && mymap.themap[ty][tx] != 5)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
 
void blinky::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void blinky::draw()
{  
	gotoxy(blinky::x, blinky::y);

	switch (mood)
	{
	case LEAVE: 
		SetConsoleTextAttribute(hConsole, blinky::color);
		std::cout << "G"; 
		break;
	case CHASE: 
		SetConsoleTextAttribute(hConsole, blinky::color);
		std::cout << "G"; 
		break;
	case SCATTER: 
		SetConsoleTextAttribute(hConsole, blinky::color);
		std::cout << "G"; 
		break;
	case PANIC: 
		SetConsoleTextAttribute(hConsole, 5);
		std::cout << "G"; 
		break;
	case EYES: 
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << "8"; 
		break;
	case WAIT:
		SetConsoleTextAttribute(hConsole, blinky::color);
		std::cout << "G";
		break;
	} 
}

void blinky::erase()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(blinky::x, blinky::y);
	SetConsoleTextAttribute(hConsole, 12); std::cout << " ";
}
 
void blinky::free(map &mymap)
{ 
	//couldve made ghosts have house exit attribute and just set it and not have to use reference..
	blinky::alive = true;
	blinky::mood = LEAVE;
	blinky::targetx = mymap.houseexit.x;
	blinky::targety = mymap.houseexit.y;
}
 
void blinky::atepellet(map &mymap)
{
	if(!blinky::alive)
	{  
		if (blinky::pelletcount > 0)
		{
			blinky::pelletcount--;
		} else 
		{  
			blinky::free(mymap);
			blinky::pelletcount = blinky::defaultpelletcount;
		}
	}
}

bool blinky::paniccheck(player &newplayer)
{
	//turn on panic
	if (newplayer.poweredup == true &&  blinky::alive == true  && blinky::mood != PANIC && blinky::mood != LEAVE && blinky::mood != EYES)
	{
		blinky::mood = PANIC;
		switch (dir)
		{
		case UP: 
			dir = DOWN;
			break;
		case LEFT:
			dir = RIGHT;
			break;
		case DOWN:
			dir = UP;
			break;
		case RIGHT: 
			dir = LEFT;
			break;
		}
		return true;
	} 
	//turn off panic mode
	if (blinky::mood == PANIC && blinky::alive == true && newplayer.poweredup == false)
	{ 
		blinky::mood = CHASE;
		return false;
	}
	return false;
}
