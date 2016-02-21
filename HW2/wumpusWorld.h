#include "wumpusComponents.h"
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
		int xGold, yGold;
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
		else cout << "Unable to open file"<<endl;

	}
  bool displayInfo(){
    system("clear");
    grid->printGrid(agent.xPos,agent.yPos,agent.orientation);
    cout<<endl;
    alive=agent.showSenses(grid);
    if(!alive)
      score -= 1000;
    cout<<"Current location: ("<<agent.xPos<<","<<agent.yPos<<")"<<endl;
    cout<<"Current Score: "<<score<<endl;
    cout<<"Last Action Selected: "<<lastAction<<endl;
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

  void greedy(){
    displayInfo();
    score += 5;
    /* We have not found the real minimum */
    while(!grid->getMinH(agent.xPos,agent.yPos)){
        displayInfo();
        score += 5;
    }
    displayInfo();
    cout<<"Final score: "<<score<<endl;
  }
};
/********************************************************/
