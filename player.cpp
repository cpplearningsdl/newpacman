#include "player.h" 

 
player::player( )
{
	player::playerx = 0;
	player::playery = 0;
	player::playerxd = 0;
	player::playeryd = 0;
	player::playerdirection = LEFT;
	player::level = 1;
	player::alive = true;
	player::lives = 4;
	player::spawncount = 0;
	player::needtospawn = false;

	player::pelletseaten = 0;
	player::atepellet = false;
	player::poweredup = false;
	player::powertime = 0;

	player::spritef = 0;
	player::sprite = '>';
	player::spriteu = 'v';
	player::spritel = '>';
	player::sprited = '^';
	player::spriter = '<';

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	player::drawplayer();
} 

void player::input()
{
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		player::sprite = player::spritel;
		player::playerdirection = LEFT;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		player::sprite = player::spriteu;
		player::playerdirection = UP;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		player::sprite = player::spriter;
		player::playerdirection = RIGHT;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		player::sprite = player::sprited;
		player::playerdirection = DOWN;
	}
}
 
bool player::walkable(map &mymap, int x, int y)
{
	if (mymap.themap[y][x] != 1 && mymap.themap[y][x] != 5)
	{
		return true;
	}
	else
	{ 
		return false;
	}
}

void player::move(map &mymap)
{
	//always start with atepellet false
	player::atepellet = false;
	if(player::alive)
	{ 
		//power pellet countdown
		if (player::powertime > 0) 
		{
			player::powertime--; 
		}
		else if (player::poweredup == true && player::powertime == 0)
			{ 
				player::poweredup = false;
			}

		//get tile we are on to check warp etc
		int tile = mymap.themap[playery][playerx];
		//tmp positions
		int tmpx = player::playerx;
		int tmpy = player::playery;
 
		switch (tile)
		{
		case 2: //pellet
			mymap.themap[playery][playerx] = 0;
			player::score = player::score + 10;
			player::pelletseaten++;
			player::atepellet = true;
			break;
			//sndPlaySound(TEXT("S102.wav"), SND_FILENAME | SND_ASYNC); 
		case 3://eat power pellet
			mymap.themap[playery][playerx] = 0;
			player::score = player::score + 50;
			player::powertime = 40;
			player::poweredup = true;
			break;
		case 8: //warp
			player::eraseplayer();
			player::playerx = mymap.warp2.x;
			player::playery = mymap.warp2.y;
			tmpx = player::playerx;
			tmpy = player::playery;
			player::drawplayer();
			break;
		case 9: //warp
			player::eraseplayer();
			player::playerx = mymap.warp.x;
			player::playery = mymap.warp.y;
			tmpx = player::playerx;
			tmpy = player::playery;
			player::drawplayer();
			break;
		}
	 
		//actually move
		switch (player::playerdirection)
		{
		case UP:
			tmpy--;
			if (walkable(mymap, tmpx, tmpy))
			{
				player::eraseplayer();
				player::playery--;
				player::drawplayer(); 
			}
			break;
		case LEFT:
			tmpx--;
			if (walkable(mymap, tmpx, tmpy))
			{
				player::eraseplayer();
				player::playerx--;
				player::drawplayer(); 
			}
			break;
		case DOWN:
			tmpy++;
			if (walkable(mymap, tmpx, tmpy))
			{
				player::eraseplayer();
				player::playery++;
				player::drawplayer(); 
			}
			break;
		case RIGHT:
			tmpx++;
			if (walkable(mymap, tmpx, tmpy))
			{
				player::eraseplayer();
				player::playerx++;
				player::drawplayer(); 
			}
			break;
		} 
	}
	else//pacman was dead, do spawn animation/spawn check
	{
		if (player::needtospawn)
		{
			player::spawn();
		}
		else { player::deathanimate(); }
	}
}
 
void player::updatefrommap(map &mymap)
{
	player::playerx = mymap.pacstart.x;
	player::playery = mymap.pacstart.y;
	player::playerxd = mymap.pacstart.x;
	player::playeryd = mymap.pacstart.y;
}
 
void player::kill() //should probably be called die to match with ghost naming 
{
	player::alive = false;
	player::needtospawn = false;
	player::spawncount = 6;
	player::lives--;   
	player::atepellet = false;
}

void player::spawn()
{ 
	player::eraseplayer();
	player::alive = true;
	player::needtospawn = false;
	player::playerx = player::playerxd;
	player::playery = player::playeryd;
	player::sprite = player::spritel; 
	player::playerdirection = LEFT;
	player::drawplayer();
}

void player::eraseplayer()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	gotoxy(playerx, playery);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << " ";
}

void player::deathanimate()
{
	//we want to animate pacman spinning after he dies so we need a countdown(spawncount) until we revive him
	player::spawncount--;
	player::drawplayer();
	if (spawncount == 0)
	{
		player::needtospawn = true;
	}
}
void player::drawplayer()
{ 
	gotoxy(playerx, playery);
	SetConsoleTextAttribute(hConsole, 14);
	//if alive use chomp animation
	if(player::alive)
	{
		if (spritef == 0)
		{
			std::cout << player::sprite;
			player::spritef = 1;
		}
		else 
		{
			std::cout << 'o'; 
			player::spritef = 0; 
		}
	}
	else
	{
		player::spritef--;//turn pacmans sprite

		//death animation
		switch (player::spritef)
		{ 
			case -1:
				player::spritef = 3;
				std::cout << player::spritel;
				break;
			case 3:
				std::cout << player::spritel;
				break;
			case 2:
				std::cout << player::sprited;
				break;
			case 1:
				std::cout << player::spriter;
				break;
			case 0:
				std::cout << player::spriteu; 
				break; 
		}
	}
}

void player::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


