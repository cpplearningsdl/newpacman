#include "Astar.h"


Astar::Astar()
{

};

Astar::~Astar()
{

}

void Astar::gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
 
	 
int Astar::findlowest(std::vector<nodescores> openlist)
{
	auto i = openlist.begin();
	int lowestindex = -1;
	int index = -1;

	
	if (!openlist.empty())
	{ 
		int lowest = i->score;
		 
		for (i; i != openlist.end(); i++)
		{
			index++;
			if (i->score <= lowest)
			{
				lowest = i->score;
				lowestindex = index;
			}
		}
	} 

	return lowestindex;

}

int Astar::findlowestclosed(std::vector<nodescores> closedlist)
{ //find lowest TOGOAL not score
	auto i = closedlist.begin() ;
	int lowestindex = -1;
	int index = -1;  
	if (!closedlist.empty())
	{
		int lowest = i->togoal;

		for (i; i != closedlist.end(); i++)
		{
			index++;
			if (i->togoal <= lowest)
			{ 
				lowest = i->togoal;
				lowestindex = index;   
			} 
		}
	} 
	//std::cout << "lowestindex togoal " << lowestindex << ":"; std::cin >> test;
	return lowestindex; 
}

int Astar::distance(int x1, int y1, int x2, int y2)
{
	int distance = (abs(x1 - x2) + abs(y1 - y2));
	return distance;
};
 
bool Astar::testwalkable(map &mymap, int x, int y, int pass, int dir, int nodir, int mood )
{ 
	//ghost room
	if (mymap.themap[y][x] ==  5)
	{
		switch (mood)//only true when eyes or leave mood
		{ 
			case CHASE: return false;
			case SCATTER: return false;
			case PANIC: return false;
			case LEAVE: return true;
			case EYES: return true;
			case WAIT: return false;
		}

	}
	//no going backwards
	if (pass == 0 && dir == nodir)
	{ 
		return false;
	}
	//wall check
	if (mymap.themap[y][x] == 1 )
	{ 
		return false;
	}
	else { return true; }
}

bool Astar::onopenlist(std::vector<nodescores> openlist, int x, int y)
{

	//check open list
	for (auto i = openlist.begin(); i != openlist.end(); i++)
	{
		if (i->x == x && i->y == y)
		{
			return true;
		}
	}
	return false;
}

bool Astar::onclosedlist(std::vector<nodescores> closedlist, int x, int y)
{
	for (auto i = closedlist.begin(); i != closedlist.end(); i++)
	{
		if (i->x == x && i->y == y)
		{
			return true;
		}
	}
	return false;
}

Astar::pos Astar::backtrack(std::vector<nodescores> closedlist, int nextx, int nexty, int startx, int starty, int goalx, int goaly)
{ 
	Astar::pos nextxy;
	nextxy.x = nextx;
	nextxy.y = nexty;
	//find the actual pos A* is for, backtrack through the closed list by using parentxy of each square as next to look for.
	for (auto searchsquare = closedlist.end()-1; searchsquare != closedlist.begin(); searchsquare--)
	{    
		if (searchsquare->x == nextxy.x && searchsquare->y == nextxy.y)
		{ 
			//gotoxy(31, 1); std::cout << "B csquare" << searchsquare->x << " " << searchsquare->y << " | start" << startx << "/" << starty;
			//gotoxy(31, 2); std::cout << "parent " << searchsquare->parentx << " " << searchsquare->parenty << "     " ;
			//std::cin >> test; 
			if (searchsquare->parentx == startx && searchsquare->parenty == starty)
			{ 
				nextxy.x = searchsquare->x;
				nextxy.y = searchsquare->y;   
				//The ultimate goal of A*  
				break;
			}
			else
			{ 
				gotoxy(searchsquare->x, searchsquare->y);
				nextxy.x = searchsquare->parentx;
				nextxy.y = searchsquare->parenty;
				closedlist.erase(searchsquare); 
 			}
			searchsquare = closedlist.end() - 1;
		}
	}   
	return nextxy;
}

Astar::pos Astar::Pathfinding(map &mymap, int x, int y, int x2, int y2, int dir, int mood)
{
	BOOL lost = true; 
	//init open and closed list
	std::vector<nodescores> closedlist;
	std::vector<nodescores> openlist;
	//start pos and end pos(and return pos)
	Astar::pos nextxy; nextxy.x = x, nextxy.y = y;
	Astar::pos start; start.x = x; start.y = y;
	Astar::pos goal; goal.x = x2; goal.y = y2;
	 

	//current square starts as start square to put on open list to start loop to find lowest value in list to test surrounding squares..
	Astar::nodescores currentsquare;
	//init currentsquare with passed in values
	currentsquare.tostart = 0;
	currentsquare.togoal = distance(start.x, start.y, goal.x, goal.y);
	currentsquare.x = start.x; currentsquare.y = start.y;
	currentsquare.parentx = 0; currentsquare.parenty = 0;
	currentsquare.score = currentsquare.togoal;
	//push square to openlist
	openlist.push_back(currentsquare);
   
	int pass = 0;
	while (lost)
	{ 
		//find lowest score on open list to test surrounding squares, returns -1 if there is no path which means we need a new goal or else we get stuck in loop forever 
		int lowestindex = findlowest(openlist);
		if(lowestindex != -1)
		{
			currentsquare = openlist.at(lowestindex);   
		}
		else
		{ //backup if path blocked,   
			lowestindex = findlowestclosed(closedlist);
			currentsquare = closedlist.at(lowestindex);  
			goal.x = currentsquare.x; 
			goal.y = currentsquare.y; 
		}  
		//found goal?! our probe found goal square, now we have to backtrack(And also check if just 1 square away and bypass backtrack)
		if (currentsquare.x == goal.x && currentsquare.y == goal.y)
		{	 
		   if (pass == 1)//step on goal instead of stop in front...
		   { 
				 nextxy.x = goal.x;
				 nextxy.y = goal.y; 
				 return nextxy;
				 break;
			}  

			//found goal square so add to closed list then backtrack to start square to get next coords
			//in backup mode this getting added twice.. 
			closedlist.push_back(currentsquare); 
			nextxy = backtrack(closedlist, currentsquare.parentx, currentsquare.parenty, start.x, start.y, goal.x, goal.y);   
			lost = false; 
			return nextxy;
			break; 
		}
		//Not goal, keep working..
		else
		{
			//test squares around current square, if walkable and not on closed list and not on openlist its added to open list
			// if its already on open list, and its score is better with current path, update its score and parentxy 
			//up  
				if (testwalkable(mymap, currentsquare.x, currentsquare.y - 1, pass, dir, 2, mood) && !onclosedlist(closedlist, currentsquare.x, currentsquare.y - 1))
				{
					int onlist = false;
					for (auto i = openlist.begin(); i != openlist.end(); i++)
					{
						if (i->x == currentsquare.x && i->y == currentsquare.y - 1)
						{
							if (i->togoal + currentsquare.tostart < i->score)
							{
								i->score = i->togoal + currentsquare.tostart;
								i->parentx = currentsquare.x;
								i->parenty = currentsquare.y;
								onlist = true;
							}
							break;
						}
					}
					if (!onlist)
					{
						nodescores tmp;
						tmp.parentx = currentsquare.x; tmp.parenty = currentsquare.y;
						tmp.x = currentsquare.x; tmp.y = currentsquare.y - 1;
						tmp.togoal = distance(currentsquare.x, currentsquare.y - 1, goal.x, goal.y);
						tmp.tostart = currentsquare.tostart + 1;
						tmp.score = tmp.togoal + tmp.tostart;
						openlist.push_back(tmp); 
					}
				} 
			//left   
				if (testwalkable(mymap, currentsquare.x - 1, currentsquare.y, pass, dir, 3, mood) && !onclosedlist(closedlist, currentsquare.x - 1, currentsquare.y))
				{
					int onlist = false;

					for (auto i = openlist.begin(); i != openlist.end(); i++)
					{
						if (i->x == currentsquare.x - 1 && i->y == currentsquare.y)
						{
							if (i->togoal + currentsquare.tostart < i->score)
							{
								i->score = i->togoal + currentsquare.tostart;
								i->parentx = currentsquare.x;
								i->parenty = currentsquare.y;
								onlist = true;
							}
							break;
						}
					}
					if (onlist == false)
					{
						//square can be added to open list as potential step
						nodescores tmp;
						tmp.parentx = currentsquare.x; tmp.parenty = currentsquare.y;
						tmp.x = currentsquare.x - 1; tmp.y = currentsquare.y;
						tmp.togoal = distance(currentsquare.x - 1, currentsquare.y, goal.x, goal.y);
						tmp.tostart = currentsquare.tostart + 1;
						tmp.score = tmp.togoal + tmp.tostart;
						openlist.push_back(tmp);

					}
				} 
			//down    
				if (testwalkable(mymap, currentsquare.x, currentsquare.y + 1, pass, dir, 0, mood) && !onclosedlist(closedlist, currentsquare.x, currentsquare.y + 1))
				{
					int onlist = false;

					for (auto i = openlist.begin(); i != openlist.end(); i++)
					{
						if (i->x == currentsquare.x && i->y == currentsquare.y + 1)
						{
							if (i->togoal + currentsquare.tostart < i->score)
							{
								i->score = i->togoal + currentsquare.tostart;
								i->parentx = currentsquare.x;
								i->parenty = currentsquare.y;
								onlist = true;
							}
							break;
						}
					}
					if (onlist == false)
					{
						nodescores tmp;
						tmp.parentx = currentsquare.x; tmp.parenty = currentsquare.y;
						tmp.x = currentsquare.x; tmp.y = currentsquare.y + 1;
						tmp.togoal = distance(currentsquare.x, currentsquare.y + 1, goal.x, goal.y);
						tmp.tostart = currentsquare.tostart + 1;
						tmp.score = tmp.togoal + tmp.tostart;
						openlist.push_back(tmp);
					}
				} 
			//right  
				if (testwalkable(mymap, currentsquare.x + 1, currentsquare.y, pass, dir, 1, mood) && !onclosedlist(closedlist, currentsquare.x + 1, currentsquare.y))
				{
					int onlist = false;

					for (auto i = openlist.begin(); i != openlist.end(); i++)
					{
						if (i->x == currentsquare.x + 1 && i->y == currentsquare.y)
						{
							if (i->togoal + currentsquare.tostart < i->score)
							{
								i->score = i->togoal + currentsquare.tostart;
								i->parentx = currentsquare.x;
								i->parenty = currentsquare.y;
								onlist = true;
							}
							break;
						}
					}
					if (onlist == false)
					{
						nodescores tmp;
						tmp.parentx = currentsquare.x; tmp.parenty = currentsquare.y;
						tmp.x = currentsquare.x + 1; tmp.y = currentsquare.y;
						tmp.togoal = distance(currentsquare.x + 1, currentsquare.y, goal.x, goal.y);
						tmp.tostart = currentsquare.tostart + 1;
						tmp.score = tmp.togoal + tmp.tostart;
						openlist.push_back(tmp);
 
					}

				} 
 
				//done testing current square, add to closed list
				closedlist.push_back(currentsquare);
				//remove current square from open list
				openlist.erase(openlist.begin() + lowestindex); 
			 
		}
		pass++;
	}
	return nextxy;
}



