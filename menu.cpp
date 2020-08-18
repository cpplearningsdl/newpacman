#include "menu.h"

using namespace std;

menu::menu()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void menu::showmenu()
{
	menu::clearscreen(); 
	gotoxy(0, 0);
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "####################\n";
	std::cout << "####################\n";
	std::cout << "##                ##\n";
	std::cout << "##                ##\n";
	std::cout << "##                ##\n";
	std::cout << "##                ##\n";
	std::cout << "##                ##\n";
	std::cout << "####################\n";
	std::cout << "####################\n";

	SetConsoleTextAttribute(hConsole, 11);
	gotoxy(4, 3);
	std::cout << "1: Map Editor";
	gotoxy(4, 4);
	std::cout << "2: New game";
	gotoxy(4, 5);
	std::cout << "3: Quit "; 
	gotoxy(4, 6);
	std::cout << "   ?";
}

 
void menu::showmapchoicemenu()
{
	menu::clearscreen();
	gotoxy(0, 0);
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "######################\n";
	std::cout << "######################\n";
	std::cout << "##                  ##\n";
	std::cout << "##                  ##\n";
	std::cout << "##                  ##\n";
	std::cout << "##                  ##\n";
	std::cout << "##                  ##\n";
	std::cout << "######################\n";
	std::cout << "######################\n";

	SetConsoleTextAttribute(hConsole, 11);
	gotoxy(4, 3);
	std::cout << "1: Default maps";
	gotoxy(4, 4);
	std::cout << "2: User maps";
	gotoxy(4, 5); 
	std::cout << "   ?";
 }

int menu::mainmenu()
{  
	int choice = -1;
	bool valid = false;

	while (!valid)
	{
		menu::showmenu();
		std::cout << std::flush;
		std::cin.clear();
		std::cin >> choice; 
		if (std::cin.good() && choice < 4 && choice > 0)
		{
			valid = true;
		}
		else
		{
			//something went wrong, we reset the buffer's state to good
			std::cout << std::flush;
			std::cin.clear();
			//and empty it 
			std::cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			//menu::mainmenu();

		}
	} 
	return choice;
}

bool menu::mapchoicemenu()
{
	int choice;
	bool valid = false;
	 
	while (!valid)
	{
		showmapchoicemenu();
		std::cout << std::flush;
		std::cin.clear(); 
		std::cin >> choice;
		//good input
		if (std::cin.good() && choice > 0 && choice < 3)
		{ 
			if(choice == 1)
			{
				return false;
			}else if(choice == 2)
			{
				return true;
			} 
		} 
	} 
	return false;
}
void menu::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void menu::clearscreen()
{
	SetConsoleTextAttribute(menu::hConsole, 0);
	menu::gotoxy(0, 0);
	for (int i = 0; i < 100; i++)
	{
		std::cout << "\n\n\n\n\n\n\n\n\n\n";
	}
	SetConsoleTextAttribute(menu::hConsole, 11);
}
