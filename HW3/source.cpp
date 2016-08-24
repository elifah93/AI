// Elifaleth Cantu Alanis
// HW 3
// Implementing Genetic Algorithm in N-Queens Problem.
// 2-29-2016

#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "ga.h"
#include "nQueens.h"

using namespace std;

int main(){
  fstream f;
  f.open ("runs.txt", fstream::in | fstream::out | fstream::app);
  f<<"\n";
  bool notFound=true;
  srand(time(0));
  int n=10;
  //cout<<"Please enter n: ";
  //cin>>n;
  GA <Nqueen> ga(n); // Initialize population.
  // It will generate 1000 generation.
  for(int i=0;i<1000;i++){
    //cout<<i<<": ";
    ga.fitnessEvalutation();
    if(ga.foundSolution(f)){ // Stop if a solution is found
      notFound=false;
      break;
    }
    ga.parentSelection();
    ga.crossover();
    ga.mutation();
    ga.fitnessEvalutationChildren();
    ga.survivorSelection();
  }
  ga.termination();
  if(notFound){
    f<< "1000 ";
    f << "not solution";
  }
  f<<"\n";
  f.close();
  return 0;
}
