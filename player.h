#pragma once
#include "constants.h"
#include "map.h"  
#include <iostream>
#include <stdio.h>
#include <windows.h> 
#pragma comment(lib, "Winmm.lib")

class player
{
public:
	 
	player( ); 
	 
	int level;
	int score;

	int playerx, playery, playerdirection;
	int playerxd, playeryd;
	bool alive;
	bool needtospawn;
	int spawncount;
	int lives;
	 
	bool atepellet;
	bool poweredup;
	int powertime;
	int pelletseaten;
	int time;

	int spritef;
	char sprite;
	char spriteu;
	char spritel;
	char sprited;
	char spriter;
	 
	void input();
	bool walkable(map &mymap, int x, int y);
	void move(map &mymap); 
	void updatefrommap(map &mymap);
	void kill();
	void spawn();
	void deathanimate();
	void eraseplayer();
	void drawplayer();
	void gotoxy(int column, int line);

	HANDLE hConsole;
};

