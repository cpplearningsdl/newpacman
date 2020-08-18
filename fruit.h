#pragma once
#include "map.h"
#include "player.h"
#include <windows.h> 
#include <iostream>




class fruit
{ 
public:
	fruit(map &mymap);

	struct position
	{
		int x, y;
	};

	bool alive;
	int pellets;
	int tillalive;
	int tilldeath;
	 
	position pos;
	position defaultpos;
	int dir;
	int value;
	int color;

	const int UP = 0;
	const int LEFT = 1;
	const int DOWN = 2;
	const int RIGHT = 3;
	
	void dofruit(map &mymap, player &newplayer);
	void addpellet();
	void spawn();
	void despawn(map &mymap);
	void move(map &mymap, player &newplayer);
	bool walkable(map &mymap, int x, int y);
	void randomadjacent(map &mymap);

	void draw();
	void erase(map &mymap); 

	void gotoxy(int column, int line); 
	HANDLE hConsole;

};

