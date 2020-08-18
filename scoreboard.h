#pragma once
#include "player.h"
#include "blinky.h"
#include "map.h"

class scoreboard
{
public:
	scoreboard(map &mymap);

	void drawscoreboard();
	void fillboard(player &newplayer, map &mymap);

	void drawmenu();

	void gotoxy(int column, int line);

	int xbuff;
	HANDLE hConsole;
};

