#pragma once
#include <limits>   
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>  
#include <iostream>

class menu
{
public:
	menu();

	void showmenu();
	bool mapchoicemenu();
	void showmapchoicemenu();
	int mainmenu();
	void gotoxy(int column, int line);
	void clearscreen();

	HANDLE hConsole;
};

