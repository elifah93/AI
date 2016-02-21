// Elifaleth Cantu Alanis
// HW2
// 2/14/2016

// I should delete the senses class from the agent and put it in the grid
// then update the print loop, so it would show the senses in that grid
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

/********** SENSE CLASS ***********************/
class Senses{
public:
	bool stench;
	bool breeze;
	bool glitter;
	bool bump;
	bool scream;

	/* Initialize all the sense to false */
	Senses(){
		stench=false;
		breeze=false;
		glitter=false;
		bump=false;
		scream=false;
	}
	/* It will set an specific sense depending of the int.
	 	The int is a code for what is int grid at coordinates x and y*/
	void setSense(string object){
		if(object == "wumpus")
			stench=true;
		else if(object=="gold")
			glitter=true;
		else if(object == "pit")
			breeze=true;
		else if(object == "wall")
			bump = true;
		else if(object == "hit")
			scream = true;
	}
	/* It would reset all the senses to false */
	void reset(){
		stench=false;
		breeze=false;
		glitter=false;
		bump=false;
		scream=false;
	}
};
/*******************************************************/
/************* CLASS SQUARE ***************************/
/* This class would be used in the grid class */
class Square{
public:
	bool wumpusDead;
	bool wumpusAlive;
	bool gold;
	bool pit;
	bool bow;
	bool arrow;
	Senses senses;

	/* Initialize variables to false because they have not been placed here */
	Square(){
		wumpusDead=false;
		wumpusAlive=false;
		gold=false;
		pit=false;
	}
	/* Set gold to true for this square */
	void putGold(){
		gold=true;
	}
	/* Set pit to true for this square */
	void putPit(){
		pit=true;
	}
	/* Set wumpusAlive to true and wumpusDead to false for this square */
	void putWumpus(){
		wumpusAlive=true;
		wumpusDead=false;
	}
	/* Set wumpusAlive to false and wumpusDead to true for this square */
	void removeWumpus(){
		wumpusAlive=false;
		wumpusDead=true;
	}
	/* Set bow to false for this square */
	void removeBow(){
		bow=false;
	}
	/* Set gold to flase for this square */
	void removeGold(){
		gold=false;
	}
	/* Set arrow to false for this square */
	void removeArrow(){
		arrow=false;
	}
	/* Set bow to true for this square */
	void putBow(){
		bow=true;
	}
	/* Set arrow to true for this square */
	void putArrow(){
		arrow=true;
	}
};
/*******************************************************/
/****************** CLASS GRID *************************/
class Grid{
public:
	int size;
	Square ** grid; /* Grid will consist of a 2 dimesion array of squares */

 /* Initialize the grid
 	Parameters: s, it would be the size of the grid */
	Grid(const int s)
	{
		size=s;
		grid= new Square *[s];
		for (int i = 0; i < s; ++i)
		{
			grid[i]= new Square[s];
		}
	}
	/* Get the new x and y according to the orientation of the agent.
	Parameters: x, the x position of the agent.
	 						y, the y position of the agent.
							orientation, the orientation of the agent. */
	bool newPosition(int &x, int &y,int orientation){
		int newX=x;
		int newY = y;
		if(orientation==0){
				newY=y-1;
		}
		else if(orientation==1){
			newY=y-1;
			newX=x+1;
		}
		else if(orientation==2){
			newX=x+1;
		}
		else if(orientation==3){
			newY=y+1;
			newX=x+1;
		}
		else if(orientation==4){
			newY=y+1;
		}
		else if(orientation==5){
			newY=y+1;
			newX=x-1;
		}
		else if(orientation==6){
			newX=x-1;
		}
		else if(orientation==7){
			newY=y-1;
			newX=x-1;
		}
		if(validPosition(newX,newY)){
			x=newX;
			y=newY;
			return true;
		}
		else
			return false;
	}
	/* Check if the (x,y) exists in the grid.
	Parameters: x, x coordinate.
							y, y coordinate. */
	bool validPosition(int x, int y){
		if((x>=size||x<0)||(y>=size||y<0))
			return false;
		else
			return true;
	}
	/* Print the grid and place the agent in the coordinates specified.
	Parameters: x, x coordinate of the agent.
	 						y, y coordinate of the agent.
							orientation, orientation of the agent.*/
	void printGrid(int x, int y, int orientation){
		int m=0;
		int n=0;
		bool agentPlaced=false;
		int coordX=0;
		int coordY=0;
		for (int i=0; i<=(size*5)+1;i++){
			for(int j=0;j<=(size*2);j++){

				if(i==size*5+1 ){
					if(j%2==1){
						cout<<"     "<<j/2<<"    ";
						continue;
					}
				}
				else if(i%5==0){
					cout<<"+ - - - - ";
					if(j==size-1){
						cout<<"+";
						break;
					}
				}
				else if(j==(size*2)||j%2==0){
					cout<<"|";
					if(i%5 == 2 && j==(size*2))
						cout<<" "<<i/5;
				}
				else{
					//cout<<j/2<<" "<<i/5<<"  ";
					if(grid[j/2][i/5].pit && i%5==2){
						cout<<"   PIT   ";
						continue;
					}
					if(grid[j/2][i/5].wumpusAlive && i%5 == 2){
						cout<<"  WUMPUS ";
						continue;
					}
					if (grid[j/2][i/5].senses.glitter && i%5 == 3 &&(!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout<<" GLITTER ";
						continue;
					}
					if (grid[j/2][i/5].senses.breeze && i%5 == 1 && (!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout <<" BREEZE  ";
						continue;
					}
					if (grid[j/2][i/5].senses.stench && i%5 == 2 && (!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout <<" STENCH  ";
						continue;
					}
					if(!agentPlaced && j/2==x && i/5==y && i%5==4){
						agentPlaced=true;
						if(orientation==0){
							cout<<"    ^    ";
							continue;
						}
						else if(orientation==1){
							cout<<"    ^>   ";
							continue;
						}
						else if(orientation==2){
							cout<<"    >    ";
							continue;
						}
						else if(orientation==3){
							cout<<"    v>   ";
							continue;
						}
						else if(orientation==4){
							cout<<"    v    ";
							continue;
						}
						else if(orientation==5){
							cout<<"    <v   ";
							continue;
						}
						else if(orientation==6){
							cout<<"    <    ";
							continue;
						}
						else if(orientation==7){
							cout<<"    <^   ";
							continue;
						}
					}
					cout<<"         ";
				}

			}
			cout<<endl;
		}
	}
	/* Return a code int which tells what is in the square.
	Parameters: x, x coordinate.
	 						y, y coordinate.
							current, optional parameter that tells if the agent is in that
								coordinates.*/
	void getSquareSense(int x, int y,Senses &sense){
		sense.glitter = grid[x][y].senses.glitter;
		sense.stench = grid[x][y].senses.stench;
		sense.breeze = grid[x][y].senses.breeze;
	}

	void getSquareObj(int x, int y, Square &sq){
		sq.wumpusAlive = grid[x][y].wumpusAlive;
		sq.wumpusDead = grid[x][y].wumpusDead;
		sq.gold = grid[x][y].gold;
		sq.pit = grid[x][y].pit;
		sq.bow = grid[x][y].bow;
		sq.arrow = grid[x][y].arrow;
	}
	/* Return the size of the grid */
	void getSize(){
		cout<<size<<endl;
	}
	/* Set gold in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void setGold(int x, int y){
		grid[x][y].putGold();
		grid[x][y].senses.setSense("gold");
	}
	/* Set pit in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void setPit(int x, int y){
		int newX, newY;
		grid[x][y].putPit();
		for(int i=0;i<8;i++){
			newX = x;
			newY = y;
			if(newPosition(newX,newY,i))
				grid[newX][newY].senses.setSense("pit");
		}
	}
	/* Set wumpus in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void setWumpus(int x, int y){
		int newX, newY;
		grid[x][y].putWumpus();
		for(int i=0;i<8;i++){
			newX = x;
			newY = y;
			if(newPosition(newX,newY,i))
				grid[newX][newY].senses.setSense("wumpus");
		}
	}
	/* Remove wumpus from the given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void wumpusDead(int x, int y){
		grid[x][y].removeWumpus();
	}
	/* Remove gold from the given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void getGold(int x, int y){
		grid[x][y].removeGold();
		grid[x][y].senses.glitter = false;
	}
	/* Remove bow from the given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void getBow(int x, int y){
		grid[x][y].removeBow();
	}
	/* Remove arrow from the given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void getArrow(int x, int y){
		grid[x][y].removeArrow();
	}
	/* Set bow in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void dropBow(int x, int y){
		grid[x][y].putBow();
	}
	/* Set arrow in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void dropArrow(int x, int y){
		grid[x][y].putArrow();
	}
	/* Set gold in given coordinates.
	Parameters: x, x coordinate.
	 						y, y coordinate.*/
	void dropGold(int x, int y){
		grid[x][y].putGold();
		grid[x][y].senses.setSense("gold");
	}
};
/*******************************************************/
/********************* CLASS AGENT ********************/
class Agent{
public:
	bool gold;
	bool bow;
	bool arrow;
	int lastItem;
	int xPos;
	int yPos;
	int orientation;
	Senses sense;
	Square square;

	/* Initialize bow and arrow to true, gold to false, and lastItem to 0,
		since he did not had anything before. */
	Agent(){
		bow=true;
		arrow=true;
		gold=false;
		lastItem=0;
	}
	/* Display in the screen what the agent percepts in the current coordinates,
		and it also returns if the agent is dead or alive.
	Parameters: g, grid that we use to check what is around the agent. */
	bool showSenses(Grid *g){
		bool alive = getSenses(g);
		if(alive){
			cout<<"Percepts: <St, Br, G, Bu, Sc> = <"<<
			sense.stench<<","<<sense.breeze<<","<<sense.glitter
			<<","<<sense.bump<<","<<sense.scream<<">"<<endl;
			sense.reset();
		}
		return alive;
	}
	/* Get what the agent sense at the the current coordinates, and it will
	 	return true if the agent can percept something (being alive) of false if
		he can not.
	Parameters: g, grid that we use to check what is around the agent. */
	bool getSenses(Grid *g){
		g->getSquareSense(xPos,yPos,this->sense);
		g->getSquareObj(xPos,yPos,this->square);
		if(this->square.wumpusAlive || this->square.pit)
			return false;
		else
			return true;
	}
	/* Set the new coordinates of the agent.
	Parameters: x, x coordinate.
	 						y: y coordinate. */
	void setPosition(int x, int y){
			this->xPos=x;
			this->yPos=y;
	}
	/* Move the agent to the direction of the orientation.
	 Parameters: g, only to use fucntions of the class Grid*/
	void move(Grid *g){
		if(!g->newPosition(xPos,yPos,orientation))
			sense.setSense("wall");
	}
	/* Shoot to the direction of the orientation.
	 Parameters: g, only to use fucntions of the class Grid*/
	void shoot(Grid *g){
		int x=this->xPos;
		int y=this->yPos;
		if(this->arrow)
		{
			this->arrow=false;
			if(g->newPosition(x,y,this->orientation)){
				g->getSquareObj(x,y,this->square);
				if(this->square.wumpusAlive){
					sense.setSense("hit");
					g->wumpusDead(x,y);
				}
			}
		}
	}
	/* Drop whatever the agent is holding.
	 Parameters: g, only to use fucntions of the class Grid*/
	void drop(Grid *g){
		if(bow){
			g->dropBow(xPos,yPos);
			bow=false;
			lastItem=1;
		}
		if(arrow){
			g->dropArrow(xPos,yPos);
			arrow=false;
		}
		if(gold){
			g->dropGold(xPos,yPos);
			gold=false;
			lastItem=2;
		}
	}/*
	/* The agent grab whatever is in the square.
	 Parameters: g, only to use fucntions of the class Grid*/
	void grab(Grid *g){
		g->getSquareObj(xPos,yPos,this->square);
		if(bow||gold)
			return;
		if(this->square.gold && this->square.bow){
			if(lastItem==1){
				gold=true;
				g->getGold(xPos,yPos);
			}
			else{
				bow=true;
				g->getBow(xPos,yPos);
				if(this->square.arrow){
					arrow = true;
					g->getArrow(xPos,yPos);
				}
			}
		}
		else if(this->square.gold){
			gold=true;
			g->getGold(xPos,yPos);
		}
		else if(this->square.bow){
			bow=true;
			g->getBow(xPos,yPos);
			if(this->square.arrow){
				arrow = true;
				g->getArrow(xPos,yPos);
			}
		}
	}
	/* Set the orientation of the agent
	Parameters: o, orientation of the agent*/
	void setOrientation(int o){
		orientation = o;
	}
};
/******************************************************************/
/******************** CLASS WORLD ********************************/
/* The class where the magic world of wumpus is created */
class World{
	Grid *grid;
	Agent agent;
	int startX,startY;
	int score;
	bool alive;
	string lastAction;

public:
	/* Read the file provided and set the wumpus, pits, gold, and all the
	characteristics of the agent. */
	void start(){
		int size, x, y, orientation,j;
		bool flagX;
		string line;
		score=0;
		alive=true;
		lastAction="NA";
  	ifstream myfile("wumpus_2.txt");
		if (myfile.is_open())
		{
			j=1;
		  while ( getline(myfile,line) )
		  {
				for(int i=0;i<line.length();i++)
				{
					if(!isspace(line[i]))
					{
						if(j==1)
						{
							//size=stoi(line);
							sscanf(line.c_str(),"%d",&size);
							if(size>10){
								cout<<"The size of the grid cannot be bigger than 10\n";
								return;
							}
							grid = new Grid(size);
						}
						else if(j==2)
						{
							if(!flagX)
							{
								x=line[i]-'0';
								flagX=true;
								continue;
							}
							else{
							 y=line[i]-'0';
							 agent.setPosition(x,y);
							 startX=x;
							 startY=y;
							 flagX=false;
						 }
						}
						else if(j==3)
						{
							agent.setOrientation(line[i]-'0');
						}
						else if(j==4)
						{
							if(!flagX)
							{
								x=line[i]-'0';
								flagX=true;
								continue;
							}
							else{
								y=line[i]-'0';
								grid->setPit(x,y);
								flagX=false;
						 	}
						}
						else if(j==5){
							if(!flagX){
								x=line[i]-'0';
								flagX=true;
								continue;
							}
							else{
								 y=line[i]-'0';
								 grid->setGold(x,y);
								 flagX=false;
						 	}
						}
						else if(j==6){
							if(!flagX){
								x=line[i]-'0';
								flagX=true;
								continue;
							}
							else{
								 y=line[i]-'0';
								 grid->setWumpus(x,y);
								 flagX=false;
							 }
						}
					}
				}
				j++;
		  }
		  myfile.close();
		}
		else cout << "Unable to open file"<<endl;

		run();
	}
	/* What gives live to the world */
	void run(){
		do
		{
			system("clear");
			grid->printGrid(agent.xPos,agent.yPos,agent.orientation);
			cout<<endl;
			alive=agent.showSenses(grid);
			if(!alive)
				score -= 1000;
			cout<<"Current location: ("<<agent.xPos<<","<<agent.yPos<<")"<<endl;
			cout<<"Current Score: "<<score<<endl;
			cout<<"Last Action Selected: "<<lastAction<<endl;
		}while(alive && Command());
		if(!alive){
			cout<<"\nYou fool are dead.\n"
			<<"Your final score is :"<<score<<endl;
		}
	}
	/* Ask to the user to choose an action. It will return false if the user
	wants to end the game. */
	bool Command(){
		string action;
		cout<<"\n"
		<<"F + Enter: move forward one square in direction of current orientation.\n"
		<<"R + Enter: turn rigth(clickwise) 45 degrees.\n"
		<<"L + Enter: turn left 45 degrees.\n"
		<<"D + Enter: drop the object the agent is currently holding.\n"
		<<"G + Enter: grab the object in the square\n"
		<<"S + Enter: shoot the arrow. This action works only the first time it is"
		<<" used. If the agent tries to use the action again, the action has no "
		<<"effect, but the -10 score is still added to the agent's score.\n"
		<<"C + Enter: climb out of the cave, This action works only when the agent is at the Start location.\n"
		<<"Q + Enter: Quit the game.\n\n"
		<<"Please enter your next action: ";
		cin>>action;

		// should use function to get the orientation
		if(action=="F"){
			score--;
			agent.move(grid);
		}
		else if(action == "R"){
			score--;
			if((agent.orientation+1)%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation((agent.orientation+1)%8);
		}
		else if(action == "L"){
			score--;
			if(agent.orientation==0)
				agent.setOrientation (7);
			else if(abs((agent.orientation-1))%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation(abs((agent.orientation-1))%8);
		}
		else if (action == "D"){
			score--;
			agent.drop(grid);
		}
		else if (action == "G"){
			score--;
			agent.grab(grid);
		}
		else if(action == "S"){
			agent.shoot(grid);
			score -= 10;
		}
		else if(action == "C"){
			score--;
			if(agent.xPos==startX&& agent.yPos==startY){
				if(agent.gold)
					score += 1000;
				cout<<"\nYou are out of the cave.\n"
				<<"Your final score is :"<<score<<endl;
			}
			return false;
		}
		else if (action == "Q") {
			return false;
		}
		else
			action="Invalid move";
		lastAction=action;
		return true;
	}
};
/********************************************************/
int main()
{
	World world; /* Create wumpus world */
	world.start(); /* Start wumpus world */
}
