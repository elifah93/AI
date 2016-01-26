// Elifaleth Cantu Alanis
// HW1
// 1/24/2016

#include <iostream>
#include <stdlib.h>
#include "string"

using namespace std;

class Agent{
	bool gold;
	bool bow;
	int xPos;
	int yPos;
};

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
};

class Square{
public:
	bool wumpus;
	bool gold;
	bool pit;
	Senses sense;

	Square(){
		this->wumpus=false;
		this->gold=false;
		this->pit=false;
	}

	void Gold(){
		gold=true;
		sense.glitter=true;
	}

	void Pit(){
		pit=true;
	}
	void Wumpus(){
		wumpus=true;
	}
	void showSenses(){
		cout<<"Percepts: <St, Br, G, Bu, Sc> = <"<<
		sense.stench<<","<<sense.breeze<<","<<sense.glitter<<","<<sense.bump<<","<<sense.scream<<">"<<endl;
	}
};

class Grid{
	int size;
	Square ** grid;

	public:
	Grid(const int s)
	{
		this->size=s;
		this->grid= new Square *[s];
		for (int i = 0; i < s; ++i)
		{
			this->grid[i]= new Square[s];	
		}
	}
	void printGrid(int x, int y, int orientation)
	{
		int m=0;
		int n=0;
		bool agentPlaced=false;
		for (int i = 0; i <= 3*this->size; ++i)
		{
			for (int j = 0; j <= 3*this->size; ++j)
			{
				if (i % 3 == 0 && j % 3 == 0)
					cout <<  "+";
				else if(j %3==0)
					cout <<"|";
				else if (j>0 && i%3==0)
					cout<<" - ";
				else
				{
					if(j/3==x&&i/3==y&&!agentPlaced)
					{
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
	void Percepts(int x, int y)
	{
		grid[x][y].showSenses();
	}
	void getSize()
	{
		cout<<this->size<<endl;
	}	
	void setGold(int x, int y)
	{
		grid[x][y].Gold();
	}
	void setPit(int x, int y)
	{
		grid[x][y].Pit();
	}
	void setWumpus(int x, int y)
	{
		grid[x][y].Wumpus();
	}
};

int main()
{
	int size;
	system("clear");
	cout<<"Enter the size of the grid: ";
	cin>>size;
	Grid grid(size);
	grid.setGold(0,0);
	grid.printGrid(0,0,2);
	cout<<endl;
	grid.Percepts(0,0);
}