#include "scoreboard.h"

scoreboard::scoreboard(map &mymap)
{
	xbuff = mymap.width+1;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void scoreboard::drawscoreboard( )
{
	gotoxy(scoreboard::xbuff, 0); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "####################";
	gotoxy(scoreboard::xbuff, 1); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "####################";
	gotoxy(scoreboard::xbuff, 2); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "##                ##";
	gotoxy(scoreboard::xbuff, 3); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "##                ##";
	gotoxy(scoreboard::xbuff, 4); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "##                ##";
	gotoxy(scoreboard::xbuff, 5); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "##                ##";
	gotoxy(scoreboard::xbuff, 6); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "####################";
	gotoxy(scoreboard::xbuff, 7); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "####################";
	gotoxy(scoreboard::xbuff, 8); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "##                ##";
	gotoxy(scoreboard::xbuff, 9); SetConsoleTextAttribute(hConsole, 9); std::cout <<  "## Score:         ##";
	gotoxy(scoreboard::xbuff, 10); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 11); SetConsoleTextAttribute(hConsole, 9); std::cout << "## Level:         ##";
	gotoxy(scoreboard::xbuff, 12); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 13); SetConsoleTextAttribute(hConsole, 9); std::cout << "## Lives:         ##";
	gotoxy(scoreboard::xbuff, 14); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 15); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 16); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 17); SetConsoleTextAttribute(hConsole, 9); std::cout << "## Pellets:       ##";
	gotoxy(scoreboard::xbuff, 18); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 19); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 20); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 21); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 22); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 23); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 24); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 25); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 26); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 27); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 28); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 29); SetConsoleTextAttribute(hConsole, 9); std::cout << "##                ##";
	gotoxy(scoreboard::xbuff, 30); SetConsoleTextAttribute(hConsole, 9); std::cout << "####################";

	gotoxy(scoreboard::xbuff+2, 3); SetConsoleTextAttribute(hConsole, 14); std::cout << "    PAC - MAN"; SetConsoleTextAttribute(hConsole, 9); 
  
}

void scoreboard::drawmenu()
{
	gotoxy(0, 0);
	//clear
	for (int y = 0; y < 100; y++)
	{
			for (int x = 0; x < 100; x++)
			{
				std::cout << " ";
			}
	}
	gotoxy(0, 0);
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "####################";
	std::cout << "####################";
	std::cout << "##                ##";
	std::cout << "##                ##";
	std::cout << "##                ##";
	std::cout << "##                ##";
	std::cout << "####################";
	std::cout << "####################";

	SetConsoleTextAttribute(hConsole, 11);
	gotoxy(7, 3);
	std::cout << "1: Newgame";
	gotoxy(7, 4);
	std::cout << "0: Exit";

}

void scoreboard::fillboard(player &newplayer, map &mymap)
{
	SetConsoleTextAttribute(hConsole, 11);

	gotoxy(scoreboard::xbuff + 9, 9); std::cout << newplayer.score  << "   ";
	gotoxy(scoreboard::xbuff + 9, 11); std::cout << newplayer.level << "   ";
	gotoxy(scoreboard::xbuff + 9, 13); std::cout << newplayer.lives << "   ";
	gotoxy(scoreboard::xbuff + 11, 17); std::cout << "    ";
	gotoxy(scoreboard::xbuff + 11, 17); std::cout << mymap.pellets - newplayer.pelletseaten;
	gotoxy(scoreboard::xbuff + 11, 18); std::cout << mymap.pellets;
	gotoxy(scoreboard::xbuff + 11, 19); std::cout << newplayer.pelletseaten;
}

void scoreboard::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}