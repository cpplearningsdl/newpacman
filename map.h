#pragma once
#include <iostream>
#include <fstream> 
#include <windows.h> 
#include <string>
#include "constants.h"
#include <experimental/filesystem>

 
 
	class map
	{
	public:
		map();

		struct pos
		{
			int x, y;
		};
		  
		int filevalid(int m, std::string filename);
		void loadmap2(int m, bool lvltype);
		void drawmap();
		void rebuild(int x, int y); 
		bool walkable(int x, int y);
		void gotoxy(int column, int line);
		void clearscreen();

		int height, width;
		int pellets;

		pos warp, warp2;
		pos houseexit;
		pos blinkypos, blinkyhome; 
		pos pinkypos, pinkyhome; 
		pos inkypos, inkyhome; 
		pos clydepos, clydehome;
		pos pacstart, fruitstart;
		 
		HANDLE hConsole;

		int themap[31][28];
	};

