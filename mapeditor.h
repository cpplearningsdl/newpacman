#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "map.h"
#include "constants.h"
#include <experimental/filesystem>
#include <iostream>
#include <fstream> 
#include <windows.h> 
#include <limits>
#include <chrono>


class mapeditor
{ 
public:
	mapeditor();

	struct pos
	{
		int x;
		int y;
	};

	void editormenu();
	void showeditormenu();
	
	int choosemaptoedit(); 
	void listfreemaps();
	void listtakenmaps();

	void savemap();
	void loadmap(int n);
	void clearmap();
	void editmap();
	void rebuild();
	void searchreplace(int n);

	bool input(); 
	void drawcursor();
	void drawselectioncursor();
	void undrawcursor( );
	void undrawselectioncursor();
	void cursorleft( );
	void cursorright( );
	void cursorup( );
	void cursordown( );
	void selectioncursorup();
	void selectioncursordown();

	
	void drawmap();
	void drawpos(int x, int y);
	void border();
	void drawcolumn(bool hide);
	void drawrow(bool hide);
	void pasteobject();
	void verticalmirrorpaste();
	void horizontalmirrorpaste();
	void pelletfill();

	void increasewidth();
	void decreasewidth();
	void increaseheight();
	void decreaseheight();

	void showtools();
	void clearscreen();
	void gotoxy(int column, int line); 
	void gotoxy(pos position);


	pos cursorpos, selectioncursorpos, zerozero;
	int height, width;
	int levelnumber;
	std::string filepath;

	int themap[MAXHEIGHT+1][MAXWIDTH+1];
 
	HANDLE hConsole;
};

