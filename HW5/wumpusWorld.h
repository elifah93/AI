#include "wumpusComponents.h"

/******************** CLASS WORLD ********************************/
/* The class where the magic world of wumpus is created */
class World{
	Grid *grid;
	Agent agent;
	bool learningState;
	int startX,startY;
	int xGold, yGold;
	double reward;
	bool alive;
	string lastAction;

public:
	/* Read the file provided and set the wumpus, pits, gold, and all the
	characteristics of the agent.
	Parameters: file, the name of the file to create wumpus world.*/
	void start(char file[],bool learningType,bool resetGrid = false){
		int size, x, y, orientation,j;
		bool flagX;
		string line;
		reward=0;
		alive=true;
		learningState = learningType;
		lastAction="NA";
  	ifstream myfile(file);
		if (myfile.is_open())
		{
			j=1;
		  while ( getline(myfile,line) ){
				for(int i=0;i<line.length();i++){
					if(!isspace(line[i])){
						if(j==1 && !resetGrid){
							cout<<endl;
							sscanf(line.c_str(),"%d",&size);
							if(size>10){
								cout<<"The size of the grid cannot be bigger than 10\n";
								return;
							}
							grid = new Grid(size);
						}
						else if(j==2){
							if(!flagX){
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
						else if(j==3){
							agent.setOrientation(line[i]-'0');
						}
						else if(j==4){
							if(!flagX){
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
			grid->placeAgent(agent.xPos,agent.yPos,agent.orientation);
		  myfile.close();
			/*for (int i = 0; i<size;i++){
        for (int j = 0; j<size;j++){
          grid->getH(i,j,xGold,yGold);
        }
      }*/
		}
		else {
			cout << "Unable to open file"<<endl;
			return;
		}

	//	run(); /*It requires user input.*/
	}
/* Display the information of wumpus world.
It could show info for user use or algorithms use.
Parameters: path, path will only work if the program is running
					either greedy or A* algorithm.*/
  /*bool displayInfo(string path=""){
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
  }*/
	//Performs the action that is required by the program
 	//It can be a movement action or a more specific one like
	//Drop, Grab or shoot.
	bool doAction(int action, bool record=false){
		bool flag = false;
		if(action==0){	// Foward
			reward--;
			agent.move(grid);
			if(record){
				grid->placeAgent(agent.xPos,agent.yPos,agent.orientation);
			}
		}
		else if(action == 1){	// Right
			reward--;
			if((agent.orientation+1)%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation((agent.orientation+1)%8);
		}
		else if(action == 2){ // Left
			reward--;
			if(agent.orientation==0)
				agent.setOrientation (7);
			else if(abs((agent.orientation-1))%8==0)
				agent.setOrientation(0);
			else
				agent.setOrientation(abs((agent.orientation-1))%8);
		}
		else if (action == 3){ // Drop
			reward--;
			agent.drop(grid);
		}
		else if (action == 4){ // Grab
			reward--;
			flag = agent.grab(grid);
			if(flag){
				reward += 20;
			}
		}
		else if(action == 5){	// Shoot
			agent.shoot(grid,reward);
			reward--;
		}
		return flag;
	}
	//Displays the Q value of each state
	void showSolution(char file[]){
		//grid->printQ();
		agent.reset();
		reward = 0;
		start(file,true);
		bool done;
		int action;
		while(true){
			action = grid->chooseAction(agent.xPos,agent.yPos,1);
			done = doAction(action,true);
			if(done){
				cout<<"rewards:"<<reward<<endl;
				f<<"rewards:"<<reward<<endl;
				grid->printGrid();
				break;
			}
		}
	}
	//Function Q learning that performs the exploring, training and find the optimal
	// action-selection  policy for a given state
	void QLearning(char file[]){
		int xOri, yOri;
		double Qmax;
		//grid->printQ();
		bool done;
		int action;
		int epo;
		srand(time(NULL));
		for (epo = 0; epo<MAX_Epochs;epo++){
			reward = 0;
			xOri = agent.xPos;
			yOri = agent.yPos;
			action = grid->chooseAction(agent.xPos,agent.yPos,1);
			done = doAction(action);
			agent.getSenses(grid,reward);
			Qmax = grid->getMaxQ(agent.xPos,agent.yPos);
			//cout<<Qmax<<endl;
			grid->UpdateFunction(xOri,yOri,action,reward,Qmax,learningState);
			//cout<<"reward: "<<reward<<endl;
			if(done){
				break;
				//agent.reset();
				//start(file,true);
			}
		}
		cout<<"Epochs: "<<epo<<endl;
		f<<"Epochs: "<<epo<<endl;
		showSolution(file);
	}
	/* What gives live to the world */
	void run(char file[]){
		QLearning(file);
	}
};
/********************************************************/
