#include "mapeditor.h"

//using namespace std;
mapeditor::mapeditor()
{
	mapeditor::zerozero.x = 0;
	mapeditor::zerozero.y = 0;
	mapeditor::cursorpos = mapeditor::zerozero;
	mapeditor::selectioncursorpos.x = MAXWIDTH+2;
	mapeditor::selectioncursorpos.y = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
}
void mapeditor::editormenu()
{ 
	bool valid = false;
	int choice = -1;
	while (!valid)
	{
		mapeditor::showeditormenu();
		std::cin >> choice;
		if (std::cin.good() && choice > 0 && choice < 3)
		{
			valid = true;
			if (choice == 1)
			{
				mapeditor::editmap();
			}
			else 
				{
					break;
				}

		}else
			{	//something went wrong, we reset the buffer's state to good
				std::cout << std::flush;
				std::cin.clear();
				//and empty it 
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

	} 
}

void mapeditor::showeditormenu()
{
	mapeditor::clearscreen();
	gotoxy(0, 0);

	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "#####################\n";
	std::cout << "#####################\n";
	std::cout << "##                 ##\n";
	std::cout << "##                 ##\n";
	std::cout << "##                 ##\n";
	std::cout << "##                 ##\n";
	std::cout << "##                 ##\n";
	std::cout << "##                 ##\n";
	std::cout << "#####################\n";
	std::cout << "#####################\n";

	SetConsoleTextAttribute(hConsole, 11);
	gotoxy(4, 3);
	std::cout << "Use Map Edit?";
	gotoxy(6, 4);
	std::cout << "1. Yes";
	gotoxy(6, 5);
	std::cout << "2. No";
	gotoxy(6, 6);
	std::cout << "? ";
}

int mapeditor::choosemaptoedit()
{ 
	mapeditor::clearscreen();
	mapeditor::listfreemaps();
	mapeditor::listtakenmaps();
	gotoxy(0, 6);
	std::cout << "Please choose any map to edit. (1-100) ";
	int choice = -1; 
	bool valid = false;
	while (!valid )
	{
		std::cout << "Enter a number: " << std::flush; 
		std::cin.clear(); 
		std::cin >> choice;
		if (std::cin.good() && choice < 101 && choice > 0)
		{ 
			valid = true; 
		}
		else
		{
			//something went wrong, we reset the buffer's state to good
			std::cout << std::flush;
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input; please re-enter." << std::endl;
		}
	}

	return choice;
}

void mapeditor::editmap()
{
	//open map for editing
	mapeditor::levelnumber = choosemaptoedit();
	mapeditor::filepath = "C:\\Users\\Keary\\source\\repos\\ConsoleApplication6\\usermaps\\" + std::to_string(mapeditor::levelnumber) + ".txt";
	//editing a old map
	if (std::experimental::filesystem::exists(mapeditor::filepath))
	{ 
		mapeditor::loadmap(mapeditor::levelnumber); 
		mapeditor::drawmap();  
	}
	else//editing a new map
	{
		mapeditor::height = 10;
		mapeditor::width = 10;
		mapeditor::clearscreen();
		mapeditor::clearmap();
		gotoxy(0, MAXHEIGHT + 2);
		std::cout << "This was a blank map and has not been saved."; 
	}
	//show tools and start an input loop(i want to keep map editor as seperate from main game as possible)
	mapeditor::showtools();
	auto timelast = std::chrono::high_resolution_clock::now();
	auto timenow = std::chrono::high_resolution_clock::now(); 

	mapeditor::drawcursor();
	mapeditor::drawselectioncursor();
	bool edit = true;

	//input loop
	while (edit)
	{ 
		timenow = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = timenow - timelast;


		if (elapsed.count() > 100)
		{
			edit = mapeditor::input();  
			timelast = timenow;
		} 
	}

	//mainloop would go here
}

void mapeditor::clearmap()
{
	mapeditor::cursorpos = mapeditor::zerozero;   
  
	for (int y = 0; y < MAXHEIGHT-1; y++)
	{
		for (int x = 0; x < MAXWIDTH-1; x++)
		{
			mapeditor::themap[y][x] = 0; 
		}
	} 
	mapeditor::drawmap();
	mapeditor::showtools();
	mapeditor::drawcursor();
	mapeditor::drawselectioncursor();
}
void mapeditor::savemap()
{ 
	std::ofstream savefile(mapeditor::filepath, std::ofstream::trunc);
	savefile << mapeditor::width << "\n";
	savefile << mapeditor::height << "\n";
	for(int y = 0; y<mapeditor::height; y++)
	{
		for (int x = 0; x < mapeditor::width; x++)
		{
			int val = mapeditor::themap[y][x];
			switch (val)
			{
			case 0: savefile  << 0;  break;
			case 1: savefile  << 1; break;
			case 2: savefile  << 2; break;
			case 3: savefile  << 3; break;
			case 4: break;
			case 5: savefile  << 5; break;
			case 6: break;
			case 7: break;
			case 8: savefile  << 8; break;
			case 9: savefile  << 9; break;
			case 18: savefile  << 'B'; break;
			case 50: savefile  << 'b'; break;
			case 32: savefile  << 'P'; break;
			case 64: savefile  << 'p'; break;
			case 25: savefile  << 'I'; break;
			case 57: savefile  << 'i'; break;
			case 19: savefile  << 'C'; break;
			case 51: savefile  << 'c'; break;
			case 35: savefile  << 'S'; break;
			case 22: savefile  << 'F'; break;
			case 40: savefile << 'X'; break;
			}
 
		}
		savefile << "\n";
	}
}
void mapeditor::loadmap(int n)
{ 
	//open the file
	std::string filename = "C:\\Users\\Keary\\source\\repos\\ConsoleApplication6\\usermaps\\" + std::to_string(n) + ".txt";
	std::ifstream mapfile(filename);
	//inputs from the file
	char val;
	std::string w, h;

	//width/height  of map are the first 2 lines of file
	std::getline(mapfile, w);
	std::getline(mapfile, h);
	mapeditor::width = std::stoi(w) ;
	mapeditor::height = std::stoi(h) ;
	//incase they manually edit.txt
	if (mapeditor::width > MAXWIDTH)
	{
		mapeditor::width = MAXWIDTH;
	}
	if (mapeditor::height> MAXHEIGHT)
	{
		mapeditor::height = MAXHEIGHT;
	}

	//load the actual game map into array
	for (int y = 0; y < mapeditor::height; y++)
	{
		for (int x = 0; x < mapeditor::width; x++)
		{
			mapfile.get(val);
			val = val - 48;
			mapeditor::themap[y][x] = val; 
		}
		mapfile.get(); //remove \n
	}
	//arbitrary restriction on minimum size
	if (mapeditor::height < 10)
	{
		mapeditor::height = 10;
	}
	if (mapeditor::width < 10)
	{
		mapeditor::width = 10;
	}
	//close file
	mapfile.close(); 
}

void mapeditor::drawmap()
{ 
	mapeditor::clearscreen();
	mapeditor::gotoxy(0, 0);

	for (int y = 0; y < mapeditor::height; y++)
	{
		for (int x = 0; x < mapeditor::width; x++)
		{
			int value = mapeditor::themap[y][x];
			switch (value)
			{
			case 0: std::cout << ' ';  break;
			case 1: SetConsoleTextAttribute(mapeditor::hConsole, 9); std::cout << '#'; break;
			case 2: SetConsoleTextAttribute(mapeditor::hConsole, 7); std::cout << '.'; break;
			case 3: SetConsoleTextAttribute(mapeditor::hConsole, 15); std::cout << 'P'; break;
			case 4: break;
			case 5: SetConsoleTextAttribute(mapeditor::hConsole, 13); std::cout << '*'; break;
			case 6: break;
			case 7: std::cout << ' '; break;
			case 8: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
			case 9: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
			case 18: SetConsoleTextAttribute(hConsole, 12); std::cout << 'B'; break;
			case 50: SetConsoleTextAttribute(hConsole, 12); std::cout << 'b'; break;
			case 32: SetConsoleTextAttribute(hConsole, 13); std::cout << 'P'; break;
			case 64: SetConsoleTextAttribute(hConsole, 13); std::cout << 'p'; break;
			case 25: SetConsoleTextAttribute(hConsole, 8); std::cout << 'I'; break;
			case 57: SetConsoleTextAttribute(hConsole, 8); std::cout << 'i'; break;
			case 19: SetConsoleTextAttribute(hConsole, 6); std::cout << 'C'; break;
			case 51: SetConsoleTextAttribute(hConsole, 6); std::cout << 'c'; break;
			case 35: SetConsoleTextAttribute(hConsole, 14); std::cout << 'S'; break;
			case 22: SetConsoleTextAttribute(hConsole, 10); std::cout << 'F'; break;
			case 40: SetConsoleTextAttribute(hConsole, 5); std::cout << 'X'; break;
			}
		}
		std::cout << "\n";
	} 
	SetConsoleTextAttribute(hConsole, 11);
	gotoxy(mapeditor::width, 0);
	std::cout << "<";
	gotoxy(0, mapeditor::height);
	std::cout << "^";
}
void mapeditor::drawpos(int x, int y)
{
	int value = mapeditor::themap[y][x];
	gotoxy(x, y);
	switch (value)
	{
	case 0: std::cout << ' ';  break;
	case 1: SetConsoleTextAttribute(mapeditor::hConsole, 9); std::cout << '#'; break;
	case 2: SetConsoleTextAttribute(mapeditor::hConsole, 7); std::cout << '.'; break;
	case 3: SetConsoleTextAttribute(mapeditor::hConsole, 15); std::cout << 'P'; break;
	case 4: break;
	case 5: SetConsoleTextAttribute(mapeditor::hConsole, 13); std::cout << '*'; break;
	case 6: break;
	case 7: std::cout << ' '; break;
	case 8: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
	case 9: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
	case 18: SetConsoleTextAttribute(hConsole, 12); std::cout << 'B'; break;
	case 50: SetConsoleTextAttribute(hConsole, 12); std::cout << 'b'; break;
	case 32: SetConsoleTextAttribute(hConsole, 13); std::cout << 'P'; break;
	case 64: SetConsoleTextAttribute(hConsole, 13); std::cout << 'p'; break;
	case 25: SetConsoleTextAttribute(hConsole, 8); std::cout << 'I'; break;
	case 57: SetConsoleTextAttribute(hConsole, 8); std::cout << 'i'; break;
	case 19: SetConsoleTextAttribute(hConsole, 6); std::cout << 'C'; break;
	case 51: SetConsoleTextAttribute(hConsole, 6); std::cout << 'c'; break;
	case 35: SetConsoleTextAttribute(hConsole, 14); std::cout << 'S'; break;
	case 22: SetConsoleTextAttribute(hConsole, 10); std::cout << 'F'; break;
	case 40: SetConsoleTextAttribute(hConsole, 5); std::cout << 'X'; break;
	}
}
void mapeditor::drawcolumn(bool hide)
{
	if (!hide)
	{ 
		for (int i = 0; i < mapeditor::height; i++)
		{
			gotoxy(mapeditor::width, i);
			SetConsoleTextAttribute(mapeditor::hConsole, 0); 
			std::cout << ' ';
		}
	} else//draw
		{
			for (int i = 0; i < mapeditor::height; i++)
			{ 
				mapeditor::drawpos(mapeditor::width-1, i);
			}
		}
}
void mapeditor::drawrow(bool hide)
{
	if (!hide)
	{
		for (int i = 0; i < mapeditor::width; i++)
		{
			gotoxy(i, mapeditor::height );
			SetConsoleTextAttribute(mapeditor::hConsole, 0); std::cout << ' ';
		}
	}
	else//draw
	{
		for (int i = 0; i < mapeditor::width; i++)
		{
			mapeditor::drawpos(i, mapeditor::height -1);
		}
	}
}

void mapeditor::listfreemaps()
{ 
	gotoxy(0, 0); 
	std::string filename; 
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "These maps already exist.\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 1; i < 100; i++)
	{
		filename = "usermaps\\" + std::to_string(i) + ".txt"; 
		if (std::experimental::filesystem::exists(filename))
		{
			std::cout << " " << i ;
		} 
	}
	std::cout << ".";
} 
void mapeditor::listtakenmaps()
{ 
	gotoxy(0, 2);
	std::string filename;
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "These maps do not exist.\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 1; i < 100; i++)
	{
		filename = "usermaps\\" + std::to_string(i) + ".txt";
		//bool exist = std::experimental::filesystem::exists(filename);
		if (!std::experimental::filesystem::exists(filename))
		{
			std::cout << " " << i;
		}
	}
	std::cout << ".";
}

void mapeditor::undrawcursor( )
{ 
	gotoxy(mapeditor::cursorpos); 
	SetConsoleTextAttribute(mapeditor::hConsole, 0);
	std::cout << " ";
	mapeditor::rebuild();
}
void mapeditor::undrawselectioncursor()
{
	gotoxy(mapeditor::selectioncursorpos);
	SetConsoleTextAttribute(mapeditor::hConsole, 0);
	std::cout << " ";
}
void mapeditor::cursorleft( )
{
	if(mapeditor::cursorpos.x > 0)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.x--; 
		mapeditor::drawcursor(); 
	}
	else if (mapeditor::cursorpos.x == 0)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.x = mapeditor::width - 1;
		mapeditor::drawcursor();
	}
}
void mapeditor::cursorright( )
{
	if (mapeditor::cursorpos.x < mapeditor::width-1)
	{ 
		mapeditor::undrawcursor();
		mapeditor::cursorpos.x++; 
		mapeditor::drawcursor();
	}
	else if (mapeditor::cursorpos.x == mapeditor::width-1)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.x = 0;
		mapeditor::drawcursor();
	}
}
void mapeditor::cursorup( )
{
	if (mapeditor::cursorpos.y > 0)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.y--; 
		mapeditor::drawcursor();
	}
	else if (mapeditor::cursorpos.y == 0)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.y = mapeditor::height - 1;
		mapeditor::drawcursor();
	}
}
void mapeditor::cursordown( )
{
	if (mapeditor::cursorpos.y < mapeditor::height - 1)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.y++; 
		mapeditor::drawcursor();
	}
	else if (mapeditor::cursorpos.y == mapeditor::height - 1)
	{
		mapeditor::undrawcursor();
		mapeditor::cursorpos.y = 0;
		mapeditor::drawcursor();
	}
}
void mapeditor::drawcursor()
{	//this is drawing the cursorpos 
	SetConsoleTextAttribute(mapeditor::hConsole, 11);  
	gotoxy(MAXWIDTH / 2-9, MAXHEIGHT + 1);
	std::cout << "                     ";
	gotoxy(MAXWIDTH / 2-9 , MAXHEIGHT + 1);
	std::cout << cursorpos.x << "/" << cursorpos.y;
	gotoxy(MAXWIDTH / 2, MAXHEIGHT + 1);
	std::cout << "Level: " << mapeditor::levelnumber;

	//the cursor
	gotoxy(mapeditor::cursorpos);
	SetConsoleTextAttribute(mapeditor::hConsole, 19);
	std::cout << " ";
	SetConsoleTextAttribute(mapeditor::hConsole, 0);
}

void mapeditor::selectioncursorup()
{
	if (mapeditor::selectioncursorpos.y > 0)
	{
		mapeditor::undrawselectioncursor();
		mapeditor::selectioncursorpos.y--;
		mapeditor::drawselectioncursor();
	}
	else if(mapeditor::selectioncursorpos.y == 0)
	{
		mapeditor::undrawselectioncursor();
		mapeditor::selectioncursorpos.y = 17;
		mapeditor::drawselectioncursor();
	}
}
void mapeditor::selectioncursordown()
{
	if (mapeditor::selectioncursorpos.y < 17)
	{
		mapeditor::undrawselectioncursor();
		mapeditor::selectioncursorpos.y++;
		mapeditor::drawselectioncursor();
	}else if(mapeditor::selectioncursorpos.y == 17)
	{
		mapeditor::undrawselectioncursor();
		mapeditor::selectioncursorpos.y = 0;
		mapeditor::drawselectioncursor();
	}
}
void mapeditor::drawselectioncursor()
{ 
	gotoxy(mapeditor::selectioncursorpos);
	SetConsoleTextAttribute(mapeditor::hConsole, 25);
	std::cout << ">";
	SetConsoleTextAttribute(mapeditor::hConsole, 0);
}

void mapeditor::pasteobject()
{
	//this pastes the selected tile on screen and updates the map array
	//since options are a const amount we just using the selection cursor .y val to determine what they want to paste
	//unless more features are added i dont see reason to make this more dynamic but it would be easy to do
	gotoxy(mapeditor::cursorpos);
	switch (mapeditor::selectioncursorpos.y)
	{
	case 0://wall 
		SetConsoleTextAttribute(mapeditor::hConsole, 9);
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
		break;
	case 1://pellet
		SetConsoleTextAttribute(mapeditor::hConsole, 7); 
		std::cout << '.'; 
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 2;
		break;
	case 2://powerpellet
		SetConsoleTextAttribute(mapeditor::hConsole, 15); 
		std::cout << 'P'; 
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 3;
		break;
	case 3://door
		SetConsoleTextAttribute(mapeditor::hConsole, 13);
		std::cout << '*';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 5;
		break;
	case 4://blinky spawn
		SetConsoleTextAttribute(hConsole, 12); 
		std::cout << 'B';
		mapeditor::searchreplace(18);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 18;
		break;
	case 5://blinky home
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << 'b';
		mapeditor::searchreplace(50);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 50;
		break;
	case 6:
		SetConsoleTextAttribute(hConsole, 13);
		std::cout << 'P';
		mapeditor::searchreplace(32);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 32;
		break;
	case 7:
		SetConsoleTextAttribute(hConsole, 13);
		std::cout << 'p';
		mapeditor::searchreplace(64);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 64;
		break;
	case 8:
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << 'I';
		mapeditor::searchreplace(25);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 25;
		break;
	case 9:
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << 'i';
		mapeditor::searchreplace(57);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 57;
		break;
	case 10:
		SetConsoleTextAttribute(hConsole, 6);
		std::cout << 'C';
		mapeditor::searchreplace(19);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 19;
		break;
	case 11:
		SetConsoleTextAttribute(hConsole, 6);
		std::cout << 'c';
		mapeditor::searchreplace(51);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 51;
		break;
	case 12:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << 'F';
		mapeditor::searchreplace(22);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 22;
		break;
	case 13:
		SetConsoleTextAttribute(hConsole, 5);
		std::cout << 'X';
		mapeditor::searchreplace(40);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 40;
		break;
	case 14:
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << 'W';
		mapeditor::searchreplace(8);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 8;
		break;
	case 15:
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << 'W';
		mapeditor::searchreplace(9);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 9;
		break;
	case 16:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << 'S';
		mapeditor::searchreplace(35);
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 35;
		break;
	case 17:
		SetConsoleTextAttribute(hConsole, 0);
		std::cout << ' ';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 0;
		break;
	}
}

void mapeditor::verticalmirrorpaste()
{
	gotoxy(mapeditor::cursorpos);
	SetConsoleTextAttribute(mapeditor::hConsole, 9);
	if(mapeditor::cursorpos.y < mapeditor::height / 2)
	{  
		//TOP
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
		//bottom
		gotoxy(mapeditor::cursorpos.x, mapeditor::height - 1 - mapeditor::cursorpos.y);
		std::cout << '#';
		mapeditor::themap[mapeditor::height - 1 - mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
	}else
	if (mapeditor::cursorpos.y > mapeditor::height / 2)
	{
		//BOTTOM
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
		//TOP
		gotoxy(mapeditor::cursorpos.x, mapeditor::height - 1 - mapeditor::cursorpos.y);
		std::cout << '#';
		mapeditor::themap[mapeditor::height - 1 - mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
	}
}
void mapeditor::horizontalmirrorpaste()
{

	gotoxy(mapeditor::cursorpos);
	SetConsoleTextAttribute(mapeditor::hConsole, 9);

	//from left side of map
	if(cursorpos.x < mapeditor::width /2)
	{  
		//left side map 
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
		//right side
		gotoxy(mapeditor::width - 1 - mapeditor::cursorpos.x, mapeditor::cursorpos.y);
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::width - 1 - cursorpos.x] = 1;
	}else//from right side
	if (cursorpos.x > mapeditor::width / 2)
	{ 
		//right side map 
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x] = 1;
		//left side
		gotoxy(mapeditor::width - 1 - mapeditor::cursorpos.x, mapeditor::cursorpos.y);
		std::cout << '#';
		mapeditor::themap[mapeditor::cursorpos.y][mapeditor::width - 1 - mapeditor::cursorpos.x] = 1;
	}


}
void mapeditor::searchreplace(int n)
{//searches array for n replaces with 0 and clears map visually
	for (int y = 0; y < MAXHEIGHT; y++)
	{
		for (int x = 0; x < MAXWIDTH; x++)
		{
			if(mapeditor::themap[y][x] == n)
			{
				mapeditor::themap[y][x] = 0;
				gotoxy(x, y);
				SetConsoleTextAttribute(hConsole, 0);
				std::cout << ' ';
			}
		}
	}
}
void mapeditor::border()
{
	//top
	for (int x = 0; x < mapeditor::width; x++)
	{
		mapeditor::themap[0][x] = 1;
		gotoxy(x, 0);
		SetConsoleTextAttribute(mapeditor::hConsole, 9);
		std::cout << '#';
	}
	//bottom
	for (int x = 0; x < mapeditor::width; x++)
	{
		mapeditor::themap[mapeditor::height-1][x] = 1;
		gotoxy(x, mapeditor::height-1);
		SetConsoleTextAttribute(mapeditor::hConsole, 9);
		std::cout << '#';
	}
	//left
	for (int y = 1; y < mapeditor::height-1; y++)
	{
		mapeditor::themap[y][0] = 1;
		gotoxy(0, y);
		SetConsoleTextAttribute(mapeditor::hConsole, 9);
		std::cout << '#';
	}
	//right
	for (int y = 1; y < mapeditor::height - 1; y++)
	{
		mapeditor::themap[y][mapeditor::width-1] = 1;
		gotoxy(mapeditor::width-1, y);
		SetConsoleTextAttribute(mapeditor::hConsole, 9);
		std::cout << '#';
	}
}
void mapeditor::pelletfill()
{
	for (int y = 0; y < mapeditor::height; y++)
	{
		for (int x = 0; x < mapeditor::width; x++)
		{
			if (mapeditor::themap[y][x] == 0)
			{
				mapeditor::themap[y][x] = 2;
				gotoxy(x, y);
				SetConsoleTextAttribute(hConsole, 7);
				std::cout << '.';
			}
		}
	}
}
void mapeditor::rebuild()
{
	//redraw map position after moving cursor
	int value = themap[mapeditor::cursorpos.y][mapeditor::cursorpos.x];
	gotoxy(mapeditor::cursorpos);

	switch (value)
	{
	case 0:  std::cout << ' ';  break;
	case 1: SetConsoleTextAttribute(hConsole, 9);  std::cout << '#'; break;
	case 2: SetConsoleTextAttribute(hConsole, 7);  std::cout << '.'; break;
	case 3: SetConsoleTextAttribute(hConsole, 15); std::cout << 'P'; break;
	case 4: break;
	case 5: SetConsoleTextAttribute(hConsole, 13); std::cout << '*'; break;
	case 6: break;
	case 7: break;
	case  8: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
	case  9: SetConsoleTextAttribute(hConsole, 11); std::cout << 'W'; break;
	case 18: SetConsoleTextAttribute(hConsole, 12); std::cout << 'B'; break;
	case 50: SetConsoleTextAttribute(hConsole, 12); std::cout << 'b'; break;
	case 32: SetConsoleTextAttribute(hConsole, 13); std::cout << 'P'; break;
	case 64: SetConsoleTextAttribute(hConsole, 13); std::cout << 'p'; break;
	case 25: SetConsoleTextAttribute(hConsole, 8);  std::cout << 'I'; break;
	case 57: SetConsoleTextAttribute(hConsole, 8);  std::cout << 'i'; break;
	case 19: SetConsoleTextAttribute(hConsole, 6);  std::cout << 'C'; break;
	case 51: SetConsoleTextAttribute(hConsole, 6);  std::cout << 'c'; break;
	case 35: SetConsoleTextAttribute(hConsole, 14); std::cout << 'S'; break;
	case 22: SetConsoleTextAttribute(hConsole, 10); std::cout << 'F'; break;
	case 40: SetConsoleTextAttribute(hConsole, 5);  std::cout << 'X'; break;
	}
}

void mapeditor::increaseheight()
{
	if (mapeditor::height < MAXHEIGHT)
	{	//erase height marker
		SetConsoleTextAttribute(mapeditor::hConsole, 0);
		gotoxy(0, mapeditor::height);
		std::cout << " ";
		//draw new height marker
		mapeditor::height++;
		SetConsoleTextAttribute(mapeditor::hConsole, 11);
		gotoxy(0, mapeditor::height);
		std::cout << "^";
		mapeditor::drawrow(true);
	}
} 
void mapeditor::increasewidth()
{
	if (mapeditor::width < MAXWIDTH)
	{	//erase width marker
		SetConsoleTextAttribute(mapeditor::hConsole, 0);
		gotoxy(mapeditor::width, 0);
		std::cout << " ";
		//draw new width marker
		mapeditor::width++;
		SetConsoleTextAttribute(mapeditor::hConsole, 11);
		gotoxy(mapeditor::width, 0);
		std::cout << "<";
		mapeditor::drawcolumn(true);
	}
}
void mapeditor::decreaseheight()
{
	if (mapeditor::height > 10)
	{
		//erase height marker
		SetConsoleTextAttribute(mapeditor::hConsole, 0);
		gotoxy(0, mapeditor::height);
		std::cout << " ";
		//draw new height marker
		mapeditor::height--;
		mapeditor::drawrow(false);
		gotoxy(0, mapeditor::height);
		SetConsoleTextAttribute(mapeditor::hConsole, 11);
		std::cout << "^";
		//cursor went ouf of bounds, move it
		if (mapeditor::cursorpos.y == mapeditor::height && cursorpos.y != 0)
		{ 
			mapeditor::cursorpos.y--;
			mapeditor::drawcursor();
		}
	}
} 
void mapeditor::decreasewidth()
{
	if (mapeditor::width > 10)
	{	//erase width marker
		SetConsoleTextAttribute(mapeditor::hConsole, 0);
		gotoxy(mapeditor::width, 0);
		std::cout << " ";
		//draw new width marker
		mapeditor::width--; 
		mapeditor::drawcolumn(false);
		gotoxy(mapeditor::width, 0);
		SetConsoleTextAttribute(mapeditor::hConsole, 11);
		std::cout << "<";
		//cursor went out of bounds move it
		if (mapeditor::cursorpos.x == mapeditor::width && cursorpos.x != 0)
		{ 
			mapeditor::cursorpos.x--;
			mapeditor::drawcursor();
		}
	}
}

void mapeditor::showtools()
{
	gotoxy(MAXWIDTH + 3, 0);
	SetConsoleTextAttribute(mapeditor::hConsole, 9); std::cout << "# Wall";  
	gotoxy(MAXWIDTH + 3, 1);
	SetConsoleTextAttribute(mapeditor::hConsole, 7); std::cout << ". Pellet"; 
	gotoxy(MAXWIDTH + 3, 2);
	SetConsoleTextAttribute(mapeditor::hConsole, 15); std::cout << "P PowerPellet"; 
	gotoxy(MAXWIDTH + 3, 3);
	SetConsoleTextAttribute(mapeditor::hConsole, 13); std::cout << "* Door(Ghosts can only go through door when looking for X)"; 
	gotoxy(MAXWIDTH + 3, 4);
	SetConsoleTextAttribute(mapeditor::hConsole, 12); std::cout << "B Blinky's spawn";
	gotoxy(MAXWIDTH + 3, 5);
	SetConsoleTextAttribute(mapeditor::hConsole, 12); std::cout << "b Blinky's home";
	gotoxy(MAXWIDTH + 3, 6);
	SetConsoleTextAttribute(mapeditor::hConsole, 13); std::cout << "P Pinky's spawn";
	gotoxy(MAXWIDTH + 3, 7);
	SetConsoleTextAttribute(mapeditor::hConsole, 13); std::cout << "p Pinky's home";
	gotoxy(MAXWIDTH + 3, 8);
	SetConsoleTextAttribute(mapeditor::hConsole, 8); std::cout << "I Inky's spawn";
	gotoxy(MAXWIDTH + 3, 9);
	SetConsoleTextAttribute(mapeditor::hConsole, 8); std::cout << "i Inky's home";
	gotoxy(MAXWIDTH + 3, 10);
	SetConsoleTextAttribute(mapeditor::hConsole, 6); std::cout << "C Clyde's spawn";
	gotoxy(MAXWIDTH + 3, 11);
	SetConsoleTextAttribute(mapeditor::hConsole, 6); std::cout << "c Clyde's home";
	gotoxy(MAXWIDTH + 3, 12);
	SetConsoleTextAttribute(mapeditor::hConsole, 10); std::cout << "F Fruit's spawn";
	gotoxy(MAXWIDTH + 3, 13);
	SetConsoleTextAttribute(mapeditor::hConsole, 1); std::cout << "X Ghost house exit(the ghost's goal to leave ghost house)";
	gotoxy(MAXWIDTH + 3, 14);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "W Warp(only 2 per level, sorry)";
	gotoxy(MAXWIDTH + 3, 15);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "W Warp(only 2 per level, sorry)";
	gotoxy(MAXWIDTH + 3, 16);
	SetConsoleTextAttribute(mapeditor::hConsole, 14); std::cout << "S  Pac spawn";
	gotoxy(MAXWIDTH + 3, 17);
	SetConsoleTextAttribute(mapeditor::hConsole, 9); std::cout << "  Empty square";
	gotoxy(MAXWIDTH + 3, 19);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << ".(Numpad) Pelletfill";
	gotoxy(MAXWIDTH + 3, 20);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "B Border Fill";
	gotoxy(MAXWIDTH + 3, 21);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "U: Clear map"; 
	gotoxy(MAXWIDTH + 3, 22);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "P: Save map";

	gotoxy(MAXWIDTH + 3, 24);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "WASD: Change map size";
	gotoxy(MAXWIDTH + 3, 25);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "RF: Change tile selection";
	gotoxy(MAXWIDTH + 3, 26);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "Arrows: Move cursor";

	gotoxy(MAXWIDTH + 3, 28);
	SetConsoleTextAttribute(mapeditor::hConsole, 11); std::cout << "ESC: Quit";
}

bool mapeditor::input()
{ 
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			mapeditor::cursorleft();
		}else
		if (GetKeyState(VK_UP) & 0x8000)
		{
			mapeditor::cursorup();
		}
		else
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			mapeditor::cursorright();
		}
		else
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			mapeditor::cursordown();
		}
		else
		if (GetKeyState(0x44) & 0x8000)
		{ 
			mapeditor::increasewidth();
		}
		else
		if (GetKeyState(0x41) & 0x8000)
		{
			mapeditor::decreasewidth();
		}
		else
		if (GetKeyState(0x53) & 0x8000)
		{
			mapeditor::increaseheight();
		}
		else
		if (GetKeyState(0x57) & 0x8000)
		{
			mapeditor::decreaseheight();
		}
		else
		if (GetKeyState(0x52) & 0x8000)
		{
			mapeditor::selectioncursorup();
		}
		else
		if (GetKeyState(0x46) & 0x8000)
		{
			mapeditor::selectioncursordown();
		}
		else
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			mapeditor::pasteobject();
		}
		else
		if (GetKeyState(0x50) & 0x8000)
		{
			mapeditor::savemap();
		}
		else
		if (GetKeyState(0x55) & 0x8000)
		{
			mapeditor::clearmap();
		}
		else
		if (GetKeyState(VK_DECIMAL) & 0x8000)
		{
			mapeditor::pelletfill();
		}
		else
		if (GetKeyState(0x42) & 0x8000)
		{
			mapeditor::border();
		}
		else
		if (GetKeyState(0x48) & 0x8000)//H
		{
			mapeditor::horizontalmirrorpaste();
		}
		else
		if (GetKeyState(0x56) & 0x8000)//V
		{
			mapeditor::verticalmirrorpaste();
		}
		else
		if (GetKeyState(VK_ESCAPE) & 0x8000)
		{
			return false;
		}
		return true;
		
 }

void mapeditor::clearscreen()
{
	SetConsoleTextAttribute(mapeditor::hConsole, 0);
	mapeditor::gotoxy(0, 0);
	for (int i = 0; i < 100; i++)
	{
		std::cout << "\n\n\n\n\n\n\n\n\n\n";
	}
	SetConsoleTextAttribute(mapeditor::hConsole, 11);
}
void mapeditor::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
} 
void mapeditor::gotoxy(pos position)
{ 
	COORD coord;
	coord.X = position.x;
	coord.Y = position.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
