#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

using namespace std;

fstream f;

#define MAX_Epochs		5000
#define MAX_Actions		6
#define EXPLORE_Rate	0.1
#define LEARNING_Rate	0.6	//Initial parameters for the algorithm
#define DISCOUNT_Rate	0.9 //which we are going to keep changing to analyze its behavior
#define getSRand()		((double)rand()/(double)RAND_MAX)
#define getRand(x)		(int)((double)(x)*rand()/(RAND_MAX+1.0))

/********** STRUCT NODE *********************/
class node{
public:
	int h;
	int g;
	int f;
	int x;
	int y;
	node * parent;
	node(){
		h = -1;
		g = 0;
		f= 0;
		parent = NULL;
	}
	node(int X, int Y){
		x = X;
		y = Y;
		h = -1;
		g = 0;
		f= 0;
		parent = NULL;
	}
};
/********************************************/

class StateAction{
public:
	double Q[MAX_Actions];
	double sumQ;
	double maxQ;
	double learning;

	StateAction(){
		for(int i=0;i<MAX_Actions;i++){
			Q[i]= 100.0;
		}
		sumQ = 100.0*MAX_Actions;
		maxQ = 0.0;
		learning = 0.9;
	}
	//Find maximum Q value of current state
	void findMaxQ(){
		maxQ = -999999.999;
		for (int i=0; i<MAX_Actions; i++){
			if(maxQ < Q[i]){
				maxQ = Q[i];
			}
		}
	}
	//Update the value of the sum variable
	void updateSum(){
		sumQ = 0.0;
		for(int i = 0; i<MAX_Actions;i++){
			sumQ += Q[i];
		}
	}
	//Generate a random action for our agent to do
	int getRandomAction(){
		int action;
		action = getRand(MAX_Actions);

		return action;
	}

	//Choose the action that our agent is going to do if the
	//Q value is acceptable
	int chooseAction(int actionSelection){
		int action;
		int count;
		double explore;
		if(actionSelection==0){
			findMaxQ();
			for (int action = 0; action<MAX_Actions;action++){
				if(maxQ == Q[action])
					return action;
			}
		}
		else{
			explore = getSRand();
			//cout<<explore<<endl;
			if(EXPLORE_Rate > explore){
				//Explore
				action = getRandomAction();
				return action;
			}
			else {
				findMaxQ();
				double prob;
				//Exploit
				action = getRandomAction();
				for(count = 0; count<MAX_Actions; count++){
					prob = (Q[action])/sumQ;
				  //cout<<"prob: "<<prob<<endl;
					if(prob>getSRand()){
						return (action);
					}
					if(++action == MAX_Actions) action = 0;
				}
				if(count == MAX_Actions){
					action = getRandomAction();
				}
			}
		}
		return (action);
	}

	void UpdateFunction(int action, int reward, double qmax,bool learningType){
		double learningValue;
		//cout << learning<<endl;
		if(learningType){
			learningValue = learning;
			learning = learning * (0.5);
		}
		else{
			learningValue = LEARNING_Rate;
		}
		// Update the maxQ
		findMaxQ();
		// Q LEARNING
		//cout<<"before: "<<Q[action]<<" ";
		Q[action] += learningValue * (reward + (DISCOUNT_Rate * qmax)-Q[action]);
		//cout<<"after: "<<Q[action]<<endl;
 		// Update the payoff sum
		updateSum();
		return;
	}
	//Print the Q value
	void printQ(){
		findMaxQ();
		cout<<"Qmax: "<<maxQ<<endl;
		for(int i =0;i<MAX_Actions;i++)
			cout<<Q[i]<<" ";
		cout<<endl;
	}
};

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
	/* Reset all the senses to false */
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
	bool agent;
	int orientation;
	bool wumpusDead;
	bool wumpusAlive;
	bool gold;
	bool pit;
	bool bow;
	bool arrow;
	StateAction state;
	Senses senses;
	//node * local;

	/* Initialize variables to false because they have not been placed here */
	Square(){
		wumpusDead=false;
		wumpusAlive=false;
		gold=false;
		pit=false;
		agent = false;
		//local = new node();
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
	/* Set agent to true for this square. */
	void placeAgent(int o){
		orientation = o;
		agent = true;
	}
	/* Calculate h */
	/*void calculateH(int x1, int y1, int x2, int y2){
		if(x1 == x2 && y1==y2)
			return;
		local->h = (max(abs(x1-x2),abs(y1-y2)))*5;
	}*/
};
/*******************************************************/
/****************** CLASS GRID *************************/
class Grid{
public:
	int size;
	Square ** grid; /* Grid will consist of a 2 dimesion array of squares */
	list <node*> Open;
	list <node*> Close;

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
	void printGrid(){
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
						f<<"     "<<j/2<<"    ";
						continue;
					}
				}
				else if(i%5==0){
					cout<<"+ - - - - ";
					f<<"+ - - - - ";
					if(j==size-1){
						cout<<"+";
						f<<"+";
						break;
					}
				}
				else if(j==(size*2)||j%2==0){
					cout<<"|";
					f<<"|";
					if(i%5 == 2 && j==(size*2)){
						cout<<" "<<i/5;
						f<<" "<<i/5;
					}
				}
				else{
					//cout<<j/2<<" "<<i/5<<"  ";
					if(grid[j/2][i/5].pit && i%5==2){
						cout<<"   PIT   ";
						f<<"   PIT   ";
						continue;
					}
					if(grid[j/2][i/5].wumpusAlive && i%5 == 2){
						cout<<"  WUMPUS ";
						f<<"  WUMPUS ";
						continue;
					}
					if (grid[j/2][i/5].senses.glitter && i%5 == 3 &&(!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout<<" GLITTER ";
						f<<" GLITTER ";
						continue;
					}
					if (grid[j/2][i/5].senses.breeze && i%5 == 1 && (!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout <<" BREEZE  ";
						f <<" BREEZE  ";
						continue;
					}
					if (grid[j/2][i/5].senses.stench && i%5 == 2 && (!grid[j/2][i/5].pit && !grid[j/2][i/5].wumpusAlive)){
						cout <<" STENCH  ";
						f <<" STENCH  ";
						continue;
					}
					if (grid[j/2][i/5].agent && i%5==4){
						if(grid[j/2][i/5].orientation==0){
							cout<<"    ^    ";
							f<<"    ^    ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==1){
							cout<<"    ^>   ";
							f<<"    ^>   ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==2){
							cout<<"    >    ";
							f<<"    >    ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==3){
							cout<<"    v>   ";
							f<<"    v>   ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==4){
							cout<<"    v    ";
							f<<"    v    ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==5){
							cout<<"    <v   ";
							f<<"    <v   ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==6){
							cout<<"    <    ";
							f<<"    <    ";
							continue;
						}
						else if(grid[j/2][i/5].orientation==7){
							cout<<"    <^   ";
							f<<"    <^   ";
							continue;
						}
					}
					cout<<"         ";
					f<<"         ";
				}
			}
			cout<<endl;
			f<<endl;
		}
	}
	/* Return the set of senses that are true in the square.
	Parameters: x, x coordinate.
	 						y, y coordinate.
							sense, this are the senses of the agent.
								We will set the senses by reference.*/
	void getSquareSense(int x, int y,Senses &sense){
		sense.glitter = grid[x][y].senses.glitter;
		sense.stench = grid[x][y].senses.stench;
		sense.breeze = grid[x][y].senses.breeze;
	}
	/* Return the set of things or objects that are true in the square.
	Parameters: x, x coordinate.
	 						y, y coordinate.
							sq, used to get the values of the things or objects in the square.*/
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
	void placeAgent(int x, int y,int o){
		grid[x][y].placeAgent(o);
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
/* Reset of the grid objects to false */
	void resetAgents(){
		for(int i = 0; i<size ; i++){
			for(int j = 0; j<size; j++){
				grid[i][j].agent = false;
			}
		}
	}

	void findMaxQ(int x, int y){
		grid[x][y].state.findMaxQ();
	}

	void updateSum(int x, int y){
		grid[x][y].state.updateSum();
	}

	void getRandomAction(int x, int y){
		cout<<grid[x][y].state.getRandomAction()<<endl;
	}

	int chooseAction(int x, int y, int actionSelection){
		return grid[x][y].state.chooseAction(actionSelection);
	}

	void UpdateFunction(int x, int y,int action, double reward,double Qmax,bool learningType){
		grid[x][y].state.UpdateFunction(action,reward,Qmax,learningType);
	}

	void printQ(){
		for (int i = 0; i<size;i++){
			for (int j = 0; j<size;j++){
				cout<<"x: "<<i<<"y: "<<j<<endl;
				grid[i][j].state.printQ();
			}
		}
	}

	double getMaxQ(int x, int y){
			grid[x][y].state.findMaxQ();
			return grid[x][y].state.maxQ;
	}
	/* Call the heuristic function of square to get h.
	Parameters: x, x coordinate of the square.
							y, y coordinate of the square.
							xGold, x coordinate for where the gold is.
							yGold, y coordinate for where the gold is. */
/*	void getH(int x, int y, int xGold, int yGold){
		grid[x][y].calculateH(x,y,xGold,yGold);
	}
	/* Return whether this is the minimum h (-1, where the gold is) of all the grid.
	Parameters: x, current x coordinate.
							y, current y coordinate. */
/*	bool getMinH(int &x, int &y){
		int minH = 99999999;
		int tmpX,tmpY;
		for(int i=0; i<8;i++){
			int newX = x;
			int newY = y;
    	if(newPosition(newX,newY,i)){
				if(grid[newX][newY].local->h < minH){
					tmpX = newX;
					tmpY = newY;
					minH = grid[tmpX][tmpY].local->h;
				}
			}
    }
		x = tmpX;
		y = tmpY;
		placeAgent(x,y);
		if(grid[x][y].local->h == -1){
			return true;
		}
		else
			return false;
	}
/* Calculate f for all the adjecent squares in the current x and y.
 	It will also keep two list an Open list and Close list. We will use these
	list for A*. It will also check if one of the adjacent sqaures has been visited.
	Parameters: x, current x location.
	 						y, current y location.
							cost, cost so far.*/
	/*void getMinF(int &x, int &y, int &cost){
		node *n;
		node *c;
		int newCost;
		if(cost == 5){
			c = new node(x,y);
			Close.push_front(c);
		}
		else{
			c = findNode(x,y);
		}
		for(int i=0; i<8;i++){
			int newX = x;
			int newY = y;
			newCost = cost;
			if(newPosition(newX,newY,i)){
				if(grid[newX][newY].senses.breeze && grid[newX][newY].senses.stench){
					newCost += 50;
				}
				else if(grid[newX][newY].senses.breeze || grid[newX][newY].senses.stench){
					newCost += 25;
				}
				else{
					newCost += 5;
				}
				if(grid[newX][newY].wumpusAlive || grid[newX][newY].pit)
					newCost += 500;
				n = new node(newX,newY);
				n->h = grid[x][y].local->h;
				n->g = newCost;
				n->f = n->h+n->g;
				n->parent = c;
				bool flag;
				node * j= new node();
				for (list<node*>::iterator it=Close.begin(); it != Close.end(); ++it){
					j=*it;
					if(n->x == j->x && n->y == j->y){
						flag = true;
						break;
					}
					flag = false;
				}
				if(!flag){
					for (list<node*>::iterator it=Open.begin(); it != Open.end(); ++it){
						j=*it;
						if(n->x == j->x && n->y == j->y){
							flag = true;
							break;
						}
					}
					if(!flag)
						Open.push_front(n);
				}
			}
		}
		n = getSmallestF();
		Close.push_front(n);
		x = n->x;
		y = n->y;
		cost = n->g;
	}
/* Gets from the Open list the node with the smallest f */
/*	node* getSmallestF(){
		node *l = new node();
		l->f = 9999999;
		node *n = new node();
		for (list<node*>::iterator it=Open.begin(); it != Open.end(); ++it){
			n= *it;
			if(n->f<l->f){
				l=n;
			}
		}
		Open.remove(l);
		return l;
	}

	/* Returns the path from the start to the goal.
	Parameters: x, x coordinate of the goal.
							y, y coordinate of the goal. */
/*	string getPath(int x, int y){
		string s="";
		string cx,cy;
		node * j = new node();
		j = findNode(x,y);

		while(j != NULL){
			stringstream ss1,ss2;
			placeAgent(j->x,j->y);
			ss1 << j->x;
			ss2 << j->y;
			cx = ss1.str();
			cy = ss2.str();
			s = "("+cx+","+cy+")"+s;
			j=j->parent;
		}
		return s;
	}

/* Returns a node or NULL depending if it found the node on either Open or Close list.
	Parameters: x, x coordinate of the node we want to find.
							y, y coordinate of the node we want to find. */
/*	node * findNode(int x, int y){
		bool flag;
		node * n = new node();
		for (list<node*>::iterator it=Close.begin(); it != Close.end(); ++it){
			n=*it;
			if(n->x == x && n->y == y){
				flag = true;
				break;
			}
			flag = false;
		}
		if(!flag){
			for (list<node*>::iterator it=Open.begin(); it != Open.end(); ++it){
				n=*it;
				if(n->x == x && n->y == y){
					flag = true;
					break;
				}
			}
		}
		if(flag)
			return n;
		else
			return NULL;
	}*/
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
	/*bool showSenses(Grid *g){
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
	bool getSenses(Grid *g, double &reward){
		g->getSquareSense(xPos,yPos,this->sense);
		g->getSquareObj(xPos,yPos,this->square);
		if(this->square.wumpusAlive || this->square.pit)
			reward -= 20;
		if(sense.breeze || sense.stench)
			reward -= 5;
		if(sense.glitter)
			reward += 10;
		//if(sense.) // check if we have the gold
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
	void shoot(Grid *g, double & reward){
		int x=this->xPos;
		int y=this->yPos;
		if(this->arrow)
		{
			this->arrow=false;
			if(g->newPosition(x,y,this->orientation)){
				g->getSquareObj(x,y,this->square);
				if(this->square.wumpusAlive){
					sense.setSense("hit");
					reward += 3;
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
	bool grab(Grid *g){
		g->getSquareObj(xPos,yPos,this->square);
		if(bow||gold)
			return gold;
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
		return gold;
	}
	/* Set the orientation of the agent
	Parameters: o, orientation of the agent*/
	void setOrientation(int o){
		orientation = o;
	}
	void reset(){
		bow=true;
		arrow=true;
		gold=false;
		lastItem=0;
	}
};
/******************************************************************/
