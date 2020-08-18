#include "que.h"
 
que::que()
{
	que::time = 0;
	que::scatter = 0;
}
que::~que()
{

}

void que::doque(blinky &newblinky, pinky &newpinky,  inky &newinky, clyde &newclyde, map &mymap)
{ 
	//add pellet to ghosts, house exit counter
	if(!newblinky.alive || !newpinky.alive || !newinky.alive || !newclyde.alive)
	{
		int ghost = nextghost(newblinky.alive, newpinky.alive, newinky.alive, newclyde.alive);
		 
		switch (ghost)
		{
			case 0: newblinky.atepellet(mymap); break;
			case 1: newpinky.atepellet(mymap); break;
			case 2: newinky.atepellet(mymap); break;
			case 3: newclyde.atepellet(mymap); break;
		} 
	}
	 //the que also handles the switch to scatter mode, probably should have had different name
	//scatter
	switch(que::time)
	{
		case 150:
			newblinky.scatter(); 
			newpinky.scatter();
			newinky.scatter();
			newclyde.scatter(); 
			break;
		case 210:
			newblinky.scatter();
			newblinky.scatter(); 
			newpinky.scatter();
			newinky.scatter();
			newclyde.scatter(); 
			break;
		case 450:
			newblinky.scatter();
			newpinky.scatter();
			newinky.scatter();
			newclyde.scatter(); 
			break;
		case 510:
			newblinky.scatter();
			newpinky.scatter();
			newinky.scatter();
			newclyde.scatter(); 
			que::time = 0;
			break;
	}

}

int que::nextghost(bool blinky, bool pinky, bool inky, bool clyde)
{
	//blinky always has priorty to leave first, clyde is the opposite
		int nextghost;
 
		if (!blinky) 
		{ 
			nextghost = 0; 
		}
		else
		if (blinky && !pinky) 
		{
			nextghost = 1; 
		}
		else
		if (blinky && pinky && !inky) 
		{
			nextghost = 2; 
		}
		else
		if (blinky && pinky && inky && !clyde) 
		{
			nextghost = 3; 
		} 

		return nextghost; 
}

 