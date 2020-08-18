#include "map.h" 
#include "mapeditor.h"
#include "pinky.h"
#include "inky.h" 
#include "blinky.h" 
#include "clyde.h"
#include "player.h" 
#include "fruit.h"
#include "que.h"
#include "scoreboard.h"
#include "menu.h"
#include <windows.h>
#include <chrono>
#include <iostream>
 
 
int main()
{
	//hide cursor 
	{ 
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO	cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursorInfo);
	}

	//timing
	auto timelast = std::chrono::high_resolution_clock::now();
	auto timenow = std::chrono::high_resolution_clock::now();
	 
	//menu
	menu newmenu;

	//start game
	bool play = true; 
	while (play)
	{ 

		//main menu/play or exit input
		int choice = newmenu.mainmenu();
		 
		if (choice == 3)//quit
		{
			play = false;
		}
		else if (choice == 1)//map editor
		{
			mapeditor newmap; 
			newmap.editmap();
		}else if( choice == 2)//play
		{
			bool leveltype = newmenu.mapchoicemenu();
			player newplayer;

			while (newplayer.level < 100)
			{ 
			//make game objects
			map mymap; 
			mymap.loadmap2(newplayer.level, leveltype);
			mymap.drawmap();

			blinky newblinky(mymap);
			pinky newpinky(mymap);
			inky newinky(mymap);
			clyde newclyde(mymap); 
			fruit newfruit(mymap); 
			que newque;
			scoreboard newscoreboard(mymap);
			newscoreboard.drawscoreboard();

			newplayer.updatefrommap(mymap); 
			newplayer.drawplayer(); 

				while (newplayer.lives > 0 && newplayer.pelletseaten < mymap.pellets)
				{
					timenow = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> elapsed = timenow - timelast;
					//player input
					newplayer.input();
					if (elapsed.count() > 350)
					{
						//player logic 
						newplayer.move(mymap);

						//time+
						newque.time++;

						//if pac eats a pellet do que and fruit stuff 
						if (newplayer.atepellet)
						{
							newque.doque(newblinky, newpinky, newinky, newclyde, mymap);
							newfruit.addpellet();
						}
						//fruit
						newfruit.dofruit(mymap, newplayer);

						//ghost logic  
						newblinky.move2(mymap, newplayer, newblinky);
						newpinky.move2(mymap, newplayer, newblinky);
						newinky.move2(mymap, newplayer, newblinky);  
						newclyde.move2(mymap, newplayer, newblinky);

						//if ghost are eyes, move twice every loop
						//if (newblinky.mood == 5) { newblinky.move2(mymap, newplayer, newblinky); }
						//if (newpinky.mood == 5) { newpinky.move2(mymap, newplayer, newblinky); }
						//if (newinky.mood == 5) { newinky.move2(mymap, newplayer, newblinky); }
						//if (newclyde.mood == 5) { newclyde.move2(mymap, newplayer, newblinky); }

						//timing
						timelast = timenow;

						//score
						newscoreboard.fillboard(newplayer, mymap); 
					} 
				}  
				//newlevel
				if (newplayer.pelletseaten == mymap.pellets)
				{
					newplayer.level++; 
					newplayer.pelletseaten = 0; 
				}
				else if (newplayer.lives == 0)//out of lives
					{
						break;//back to main menu loop
					}
			}

		}
	}
}

 