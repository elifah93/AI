// Elifaleth Cantu Alanis
// HW1
// 1/24/2016

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

	Senses(){
		this->stench=false;
		this->breeze=false;
		this->glitter=false;
		this->bump=false;
		this->scream=false;
	}
	void setSense(int x){
		if (x==0)
			return;
		if(x==1){
			this->glitter=true;
			this->stench=true;
		}
		else if(x==2)
			this->stench=true;
		else if(x==3)
			this->glitter=true;
		else if(x==4)
			this->breeze=true;
		else if(x==5)
			this->scream=true;
		else if(x==6)
			this->bump=true;
	}
	void reset(){
		this->stench=false;
		this->breeze=false;
		this->glitter=false;
		this->bump=false;
		this->scream=false;
	}
};
/*******************************************************/
/************* CLASS SQUARE ***************************/
class Square{
public:
	bool wumpus;
	bool gold;
	bool pit;

	Square(){
		this->wumpus=false;
		this->gold=false;
		this->pit=false;
	}
	void Gold(){
		gold=true;
	}
	void Pit(){
		pit=true;
	}
	void Wumpus(){
		wumpus=true;
	}
};
/*******************************************************/
/****************** CLASS GRID *************************/
class Grid{
public:
	int size;
	Square ** grid;

	Grid(const int s)
	{
		this->size=s;
		this->grid= new Square *[s];
		for (int i = 0; i < s; ++i)
		{
			this->grid[i]= new Square[s];
		}
	}
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
	bool validPosition(int x, int y){
		if((x>=this->size||x<0)||(y>=this->size||y<0))
			return false;
		else
			return true;
	}
	void printGrid(int x, int y, int orientation){
		int m=0;
		int n=0;
		bool agentPlaced=false;
		int coordX=0;
		int coordY=0;
		for (int i = 0; i <= 3*this->size+1; ++i){
			for (int j = 0; j <= 3*this->size+1; ++j){
				if(j==3*this->size+1){
					if(coordX==2){
						cout<<" "<<i/3;
						coordX=0;
						break;
					}
					coordX++;
				}
				else if (i==3*this->size+1){
					if(j/3==this->size)
						break;
					else if(j % 3 == 0)
						cout<<"   "<<j/3<<"   ";
				}
				else if (i % 3 == 0 && j % 3 == 0)
					cout <<  "+";
				else if(j %3==0)
					cout <<"|";
				else if (j>0 && i%3==0)
					cout<<" - ";
				else{
					if(j/3==x&&i/3==y&&!agentPlaced){
						agentPlaced=true;
						if(orientation==0)
							cout<<" ^ ";
						else if(orientation==1)
							cout<<" ^>";
						else if(orientation==2)
							cout<<" > ";
						else if(orientation==3)
							cout<<" v>";
						else if(orientation==4)
							cout<<" v ";
						else if(orientation==5)
							cout<<" <v";
						else if(orientation==6)
							cout<<" < ";
						else if(orientation==7)
							cout<<" <^";
						else
							cout<<"   ";
					}
					else
						cout<<"   ";
				}
			}
	        cout <<"\n";
		}
	}
	int getSquare(int x, int y, bool current = false){
		if(this->grid[x][y].wumpus && this->grid[x][y].gold && current)
			return 1;
		else if(this->grid[x][y].wumpus)
			return 2;
		else if(this->grid[x][y].gold && current)
			return 3;
		else if(this->grid[x][y].pit)
			return 4;
		else
			return 0;
	}
	void getSize(){
		cout<<this->size<<endl;
	}
	void setGold(int x, int y){
		this->grid[x][y].Gold();
	}
	void setPit(int x, int y){
		this->grid[x][y].Pit();
	}
	void setWumpus(int x, int y){
		this->grid[x][y].Wumpus();
	}
};
/*******************************************************/
/********************* CLASS AGENT ********************/
class Agent{
public:
	bool gold;
	bool bow;
	bool arrow;
	int xPos;
	int yPos;
	int orientation;
	Senses sense;

	Agent(){
		this->bow=true;
		this->arrow=true;
	}
	void showSenses(Grid *g){
		getSenses(g);
		cout<<"Percepts: <St, Br, G, Bu, Sc> = <"<<
		sense.stench<<","<<sense.breeze<<","<<sense.glitter
		<<","<<sense.bump<<","<<sense.scream<<">"<<endl;
		sense.reset();
	}
	void getSenses(Grid *g){
		int x;
		int y;
		for(int i=0;i<=8;i++){
			x=this->xPos;
			y=this->yPos;
			if(i==8)
				sense.setSense(g->getSquare(x,y,true));
			else if(g->newPosition(x,y,i))
				sense.setSense(g->getSquare(x,y));
		}
	}
	void setPosition(int x, int y){
			this->xPos=x;
			this->yPos=y;
	}
	void move(Grid *g){
		if(!g->newPosition(this->xPos,this->yPos,this->orientation))
			sense.setSense(6);
	}
	void shoot(Grid *g){
		if(this->arrow)
		{
			this->arrow=false;
		}
	}
	void setOrientation(int orientation){
		this->orientation = orientation;
	}
};
/******************************************************************/
/******************** CLASS WORLD ********************************/
class World{
	Grid *grid;
	Agent agent;

public:
	void start(){
		int size, x, y, orientation,j;
		bool flagX;
		string line;
  	ifstream myfile("wumpus_1.txt");
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
							size=line[i]-'0';
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
	void run(){
		do
		{
			system("clear");
			grid->printGrid(agent.xPos,agent.yPos,agent.orientation);
			cout<<endl;
			agent.showSenses(grid);
			// show score
		}while(Command());
	}

	bool Command(){
		char action;
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
		if(action=='F'){
			agent.move(grid);
		}
		else if(action == 'R'){
			if((agent.orientation+1)%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation((agent.orientation+1)%8);
		}
		else if(action == 'L'){
			if(agent.orientation==0)
				agent.setOrientation (7);
			else if(abs((agent.orientation-1))%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation(abs((agent.orientation-1))%8);
		}
		else if(action == 'S'){
			agent.shoot(grid);
		}
		else if (action == 'Q') {
			return false;
		}
		return true;
	}
};
/********************************************************/

int main()
{
	World world;
	world.start();
}
