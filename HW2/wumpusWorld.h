#include "wumpusComponents.h"

/******************** CLASS WORLD ********************************/
/* The class where the magic world of wumpus is created */
class World{
	Grid *grid;
	Agent agent;
	int startX,startY;
	int xGold, yGold;
	int score;
	bool alive;
	string lastAction;

public:
	/* Read the file provided and set the wumpus, pits, gold, and all the
	characteristics of the agent.
	Parameters: file, the name of the file to create wumpus world.*/
	void start(char file[]){
		int size, x, y, orientation,j;
		bool flagX;
		string line;
		score=0;
		alive=true;
		lastAction="NA";
  	ifstream myfile(file);
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
							 grid->placeAgent(x,y);
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
								 xGold = x;
								 yGold = y;
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
			for (int i = 0; i<size;i++){
        for (int j = 0; j<size;j++){
          grid->getH(i,j,xGold,yGold);
        }
      }
		}
		else {
			cout << "Unable to open file"<<endl;
			return;
		}

		//world.run(); /*It requires user input.*/
		cout<<"\n GREEDY"<<endl;
		greedy(); /* Implementing greedy algortihm*/
		cout<<"\n A* "<<endl;
		aStar(); /* Implementing A* algortihm */
	}
/* Display the information of wumpus world.
It could show info for user use or algorithms use.
Parameters: path, path will only work if the program is running
					either greedy or A* algorithm.*/
  bool displayInfo(string path=""){
    system("clear");
    grid->printGrid(agent.xPos,agent.yPos,agent.orientation);
    cout<<endl;
		if(path == ""){
			alive=agent.showSenses(grid);
	  	if(!alive)
	    	score -= 1000;
	  	cout<<"Current location: ("<<agent.xPos<<","<<agent.yPos<<")"<<endl;
	  	cout<<"Current Score: "<<score<<endl;
	  	cout<<"Last Action Selected: "<<lastAction<<endl;
		}
		else{
			cout<<"Path: "<<path<<endl;
			cout<<"Cost: "<<score<<endl;
		}
  }
	/* What gives live to the world */
	void run(){
		do
		{
      displayInfo();
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

/* Implementing AI greedy algortihm.*/
  void greedy(){
		string path="";
		string x,y;
		score = 0;
		if(startX!=xGold && startY != yGold){
			agent.xPos = startX;
			agent.yPos = startY;
			grid->resetAgents();
			grid->placeAgent(agent.xPos, agent.yPos);
			stringstream ss1,ss2;
			ss1 << agent.xPos;
			ss2 << agent.yPos;
			string x = ss1.str();
			string y = ss2.str();
			path = path+"("+x+","+y+")";
		  score += 5;
		  /* We have not found the real minimum */
		  while(!grid->getMinH(agent.xPos,agent.yPos)){
				stringstream ss1,ss2;
				ss1 << agent.xPos;
				ss2 << agent.yPos;
				x = ss1.str();
				y = ss2.str();
				path = path+"("+x+","+y+")";
		    score += 5;
		  }
			stringstream ss3,ss4;
			ss3 << agent.xPos;
			ss4 << agent.yPos;
			x = ss3.str();
			y = ss4.str();
			path = path+"("+x+","+y+")";
		}
    displayInfo(path);
  }
/* Implementing AI A* algortihm */
	void aStar(){
		string path;
		score = 0;
		if(startX!=xGold && startY != yGold){
			agent.xPos = startX;
			agent.yPos = startY;
			grid->resetAgents();
			grid->placeAgent(agent.xPos, agent.yPos);
			while(true){
				score += 5;
				grid->getMinF(agent.xPos,agent.yPos,score);
				if(agent.xPos == xGold && agent.yPos == yGold)
					break;
			}
			path=grid->getPath(xGold,yGold);
		}
		displayInfo(path);
	}
};
/********************************************************/
