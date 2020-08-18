#pragma once
#include "map.h"
#include "vector" 

 
class Astar
{
public:
	Astar();
	~Astar();


	struct pos
	{
		int x, y;
	};

	static pos Pathfinding(map &mymap, int x, int y, int x2, int y2, int dir, int mood);
 
private:
	 
	struct nodescores
	{
		int score, togoal, tostart, x, y, parentx, parenty;
	}; 
	static void gotoxy(int column, int line);
	static int findlowest(std::vector<nodescores> openlist);
	static int findlowestclosed(std::vector<nodescores> closedlist);

	static int distance(int x, int y, int x2, int y2);

	static pos backtrack(std::vector<nodescores> closedlist, int nextx, int nexty, int startx, int starty, int goalx, int goaly);

	static bool testwalkable(map &mymap, int x, int y, int pass, int dir, int nodir, int mood);

	static bool onopenlist(std::vector<nodescores> openlist, int x, int y);

	static bool onclosedlist(std::vector<nodescores> closedlist, int x, int y);


};

