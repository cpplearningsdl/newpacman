#include "ghostque.h"


const int BLINKY = 0; 
 
ghostque::ghostque()
{
	ghostque::curghost = BLINKY; 
}

ghostque::~ghostque()
{

}

void ghostque::atepellet(blinky &newblinky)
{
	switch (ghostque::curghost)
	{
		case BLINKY:
			newblinky.pellet();
			break; 
	} 	
} 
