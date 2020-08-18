#include "map.h"


	map::map()
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	int map::filevalid(int m, std::string filename)
	{
		//set to random default map if map doesnt exist
		filename = filename + std::to_string(m) + ".txt";
		if (std::experimental::filesystem::exists(filename))
		{ 
			return m;
		}
		else
			{
				m = rand() % 2 + 1;
				return m;
			} 
	}
	 //loadmap(level) from file, file must be level number; 1, 2, 3, etc.
	void map::loadmap2(int m, bool lvltype)
	{
		//m is the level number/file name, still needs directory tho so filename string
		std::string filename;
 
		//open the file
		//user or default
		if(lvltype == false)
		{
			filename = "defaultmaps\\";
		}else
		{
			filename = "usermaps\\";
		}

		//need check here if map exists
		m = filevalid(m, filename);

		filename = filename + std::to_string(m) + ".txt";
		 
		std::ifstream mapfile(filename);
		//inputs from the file
		char val; 
		std::string w, h;

		//height/width of map are the first 2 lines of file
		std::getline(mapfile, w);
		std::getline(mapfile, h); 
		map::width = std::stoi(w);
		map::height = std::stoi(h);  
		if (map::width > MAXWIDTH)
		{
			map::width = MAXWIDTH;
		}
		if (map::height > MAXHEIGHT)
		{
			map::height = MAXHEIGHT;
		}
		 
		//load the actual game map into array, also set all the special points like ghost default positions, fruit, player, etc
		for (int y = 0; y < map::height; y++)
		{
			for (int x = 0; x < map::width; x++)
			{
				mapfile.get(val);
				val = val - 48;
				map::themap[y][x] = val;
 
				//set warps, ghost start positions, and their home positions(Scatter targets) unfortunately no way to set which direction ghost should face in default, which could lead to problem if you
				//set up the ghosthouse in a particular way!
				switch (val)
				{
				case 2:
					map::pellets++;
					break;
				case 8:
					map::warp.x = x;  map::warp.y = y; 
					break;
				case 9:
					map::warp2.x = x;  map::warp2.y = y; 
					break;
				case 18://B
					map::blinkypos.x = x; map::blinkypos.y = y;
					map::themap[y][x] = 0;
					break;
				case 50://b
					map::blinkyhome.x = x; map::blinkyhome.y = y;
					map::themap[y][x] = 1;
					break;
				case 32: //P
					map::pinkypos.x = x; map::pinkypos.y = y;
					map::themap[y][x] = 0;
					break;
				case 64: //p
					map::pinkyhome.x = x; map::pinkyhome.y = y;
					map::themap[y][x] = 1;
					break;
				case 25://I
					map::inkypos.x = x; map::inkypos.y = y;
					map::themap[y][x] = 0;
					break;
				case 57://i
					map::inkyhome.x = x; map::inkyhome.y = y;
					map::themap[y][x] = 1;
					break;
				case 19://C
					map::clydepos.x = x; map::clydepos.y = y;
					map::themap[y][x] = 0;
					break;
				case 51://c
					map::clydehome.x = x; map::clydehome.y = y;
					map::themap[y][x] = 1;
					break;
				case 35://S
					map::pacstart.x = x; map::pacstart.y = y;
					map::themap[y][x] = 0;
					break;
				case 22://F
					map::fruitstart.x = x; map::fruitstart.y = y;
					map::themap[y][x] = 2;
					map::pellets++;
					break;
				case 40://X
					map::houseexit.x = x; map::houseexit.y = y;
					map::themap[y][x] = 0;
					break;
				}
			}
			mapfile.get(); //remove \n
		} 
		//close file
		mapfile.close();
	} 

	void map::drawmap()
	{ 
		map::clearscreen();
		 map::gotoxy(0, 0);
		for (int y = 0; y < map::height; y++)
		{
			for (int x = 0; x < map::width; x++)
			{ 
				int value = themap[y][x]; 
				switch (value)
				{
					case 0: std::cout << ' ';  break;
					case 1: SetConsoleTextAttribute(hConsole, 9); std::cout << '#'; break;
					case 2: SetConsoleTextAttribute(hConsole, 7); std::cout << '.';break;
					case 3: SetConsoleTextAttribute(hConsole, 15); std::cout << 'P'; break;
					case 4: break;
					case 5: SetConsoleTextAttribute(hConsole, 13); std::cout << '*'; break;
					case 6: break;
					case 7: std::cout << ' '; break;
					case 8: std::cout << ' '; break;
					case 9: std::cout << ' '; break; 
				}
			}
			std::cout << "\n";
		}
	}

	//rebuild map after ghosts erase things they step on
	void map::rebuild(int x, int y)
	{  
		int value = themap[y][x];
		gotoxy(x, y); 

		switch (value)
		{
			case 0:  std::cout << ' ';  break;
			case 1: SetConsoleTextAttribute(hConsole, 9); std::cout << '#'; break;
			case 2: SetConsoleTextAttribute(hConsole, 7); std::cout << '.'; break;
			case 3: SetConsoleTextAttribute(hConsole, 15); std::cout << 'P'; break;
			case 4: break;
			case 5: SetConsoleTextAttribute(hConsole, 13); std::cout << '*'; break;
			case 6: break;
			case 7: std::cout << ' '; break;
			case 8: std::cout << ' '; break;
			case 9: std::cout << ' '; break;
			case 'B': std::cout << ' '; break;//technically al these should be impossible.. need to check
			case 'b': std::cout << ' '; break;
			case 'P': std::cout << ' '; break;
			case 'p': std::cout << ' '; break;
			case 'I': std::cout << ' '; break;
			case 'i': std::cout << ' '; break;
			case 'C': std::cout << ' '; break;
			case 'c': std::cout << ' '; break;
			case 'S': std::cout << ' '; break; 
			case 'F': std::cout << ' '; break; 
			case 'L': std::cout << ' '; break;
		} 
	}
	  
	bool map::walkable(int x, int y)
	{ 
		if (themap[y][x] != 1 && themap[y][x] != 5)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//set cursor to console x,y position
	void map::gotoxy(int column, int line)
	{
		COORD coord;
		coord.X = column;
		coord.Y = line;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void map::clearscreen()
	{
		SetConsoleTextAttribute(hConsole, 0);
		map::gotoxy(0, 0);
		for (int i = 0; i < 100; i++)
		{
			std::cout << "\n\n\n\n\n\n\n\n\n\n";
		}
		SetConsoleTextAttribute(map::hConsole, 11);
	}
