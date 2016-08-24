// Elifaleth Cantu Alanis
// HW4
// 4/16/2016

#include "wumpusWorld.h"
int main()
{
	char file[50];
	cout<<"Please enter the name of the file that we will use to create the wumpus world: ";
	cin>>file;
	World world; /* Create wumpus world */
	world.start(file); /* Start wumpus world.*/
	return 0;
}
