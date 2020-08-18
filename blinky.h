#pragma once
#include "constants.h"
#include "map.h"
#include "player.h"
#include "Astar.h"
#include <windows.h> 
#include <iostream>

 
class blinky
	{
	public:
		blinky(map &mymap); 
		blinky();
		//ghost attributes
		int mood, oldmood;
		int x, y, dir;
		int defaultx, defaulty, defaultdir;
		int color;
		int homex, homey;
		int targetx, targety;
		int oldtargetx, oldtargety;
		int pelletcount;
		int defaultpelletcount;
		bool alive;

		void reset();
		void die();
		void free(map &mymap);

		 //movement
		void move2(map &mymap, player &newplayer, blinky &newblinky);
		virtual void settarget(map &mymap, player &newplayer); 
		bool walkable(map &mymap, int tx, int ty); 
		int distance(int x1, int y1, int x2, int y2);
		Astar::pos randomadjacent(map &mymap);
		 
		bool eatpac(player &newplayer);
		bool paniccheck(player &newplayer); 
		void atepellet(map &mymap);
		void scatter();
		 
		//rendering
		void gotoxy(int column, int line);
		void draw();
		void erase(); 
		HANDLE hConsole;
 
	};

