#include "fruit.h"

fruit::fruit(map &mymap)
{ 
	fruit::pos.x = mymap.fruitstart.x;
	fruit::pos.y = mymap.fruitstart.y;
	fruit::defaultpos.x = mymap.fruitstart.x;
	fruit::defaultpos.y = mymap.fruitstart.y;
	fruit::dir = 0;
	fruit::color = 10;

	fruit::alive = false;
	fruit::pellets = 0;
	fruit::tilldeath = 10;

	fruit::value = 500;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


void fruit::dofruit(map &mymap, player &newplayer)
{	 
	//not alive
	if (!fruit::alive)
	{
		//spawn fruit
		if (fruit::pellets == 17 || fruit::pellets == 47 )
		{
			fruit::spawn();
		}
	}
	else//already alive
	{ 
		//get eaten
		if (fruit::pos.x == newplayer.playerx && fruit::pos.y == newplayer.playery)
		{
			newplayer.score = newplayer.score + fruit::value; 
			fruit::tilldeath = 0;//so we dont despawn twice
			newplayer.drawplayer();
		} 
		//didnt eaten(this isn't an else so there are no edge cases of pacman moving onto us when we are at tildeath==0 and we despawn instead of get eaten
		if (fruit::tilldeath > 0)
		{  	//move
			fruit::move(mymap, newplayer);
			fruit::tilldeath--; 
			//get eaten, might as well check before and after moving
			if (fruit::pos.x == newplayer.playerx && fruit::pos.y == newplayer.playery)
			{
				newplayer.score = newplayer.score + fruit::value; 
				fruit::despawn(mymap);//this is despawn instead of tilldeath=0 so we don't get eaten twice
				newplayer.drawplayer();
			}
		}
		else
		{	//despawn 
			fruit::despawn(mymap);
		}
	}
};

void fruit::addpellet()
{
	fruit::pellets++;
}

void fruit::spawn()
{
	//reset values
	fruit::pos.x = fruit::defaultpos.x;
	fruit::pos.y = fruit::defaultpos.y;
	fruit::tilldeath = 220;
	fruit::alive = true; 
	fruit::draw(); 
}

void fruit::despawn(map &mymap)
{
	gotoxy(fruit::pos.x, fruit::pos.y);
	mymap.rebuild(fruit::pos.x, fruit::pos.y);
	fruit::tilldeath = 0;
	fruit::alive = false;
}

void fruit::move(map &mymap, player &newplayer)
{    
	int tile = mymap.themap[fruit::pos.y][fruit::pos.x];
	fruit::erase(mymap);

	//warp 
	if (tile == 8)
	{ 
		fruit::pos.x = mymap.warp2.x;
		fruit::pos.y = mymap.warp2.y;  
	}
	if (tile == 9)
	{ 
		fruit::pos.x = mymap.warp.x;
		fruit::pos.y = mymap.warp.y;  
	}

	fruit::randomadjacent(mymap); 
	fruit::draw();
}

bool fruit::walkable(map &mymap, int x, int y)
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

void fruit::randomadjacent(map &mymap)
{ 
	 int valid = 0;
	 int validlist[3];
	 int random;
	 //up
	 if (walkable(mymap, fruit::pos.x, fruit::pos.y - 1) && fruit::dir != DOWN)
	 {
		 validlist[valid] = 0;
		 valid = valid++;
	 }
	 //left
	 if (walkable(mymap, fruit::pos.x - 1, fruit::pos.y) && fruit::dir != RIGHT)
	 {
		 validlist[valid] = 1;
		 valid = valid++;
	 }
	 //down
	 if (walkable(mymap, fruit::pos.x, fruit::pos.y + 1) && fruit::dir != UP)
	 {
		 validlist[valid] = 2;
		 valid = valid++;
	 }
	 //right
	 if (walkable(mymap, fruit::pos.x + 1, fruit::pos.y) && fruit::dir != LEFT)
	 {
		 validlist[valid] = 3;
		 valid = valid++;
	 }

	 if (valid == 0)//incase dead end, normally nothing in pacman can go 'backwards' unless panicd but if you let users make maps..
	 {
		 random = rand() % 3;
	 }
	 else 
	 {
		 random = rand() % valid;
		 random = validlist[random];
	 }

	 switch (random)
	 {
	 case 0:
		 fruit::pos.y--;
		 fruit::dir = UP;
		 break;
	 case 1:
		 fruit::pos.x--;
		 fruit::dir = LEFT;
		 break;
	 case 2:
		 fruit::pos.y++;
		 fruit::dir = DOWN;
		 break;
	 case 3:
		 fruit::pos.x++;
		 fruit::dir = RIGHT;
	 } 
}

void fruit::draw()
{  
	gotoxy(fruit::pos.x, fruit::pos.y);
	SetConsoleTextAttribute(hConsole, fruit::color);
	std::cout << "Q";
	//advance color for blink effect
	fruit::color++;
	if (fruit::color > 13)
	{
		fruit::color = 10;
	}
}

void fruit::erase(map &mymap)
{  
	mymap.rebuild(fruit::pos.x, fruit::pos.y);
}
  
void fruit::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
