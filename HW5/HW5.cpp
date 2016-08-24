// Elifaleth Cantu Alanis
// HW2
// 2/14/2016

#include "wumpusWorld.h"
int main()
{
	f.open ("output.txt", fstream::in | fstream::out | fstream::app);
	f<<"\n\n";
	char file[50];
	int input;
	bool type;
	cout<<"Please enter the name of the file that we will use to create the wumpus world: ";
	cin>>file;
	cout<<"Choose type of learning 1: same value for all states; 2: variable learning rate that is a function of the number of time a state is visited. ";
	cin>>input;
	if(input == 1){
		f << "Learning rate: "<<LEARNING_Rate<<", Discount Rate: "<<DISCOUNT_Rate<<endl;
		type = false;
	}
	else{
		f << "Discount Rate: "<<DISCOUNT_Rate<<endl;
		type = true;
	}
	World world; /* Create wumpus world */
	world.start(file,type); /* Start wumpus world.*/
	world.run(file); /*It requires user input.*/
	return 0;
}
