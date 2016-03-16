// Elifaleth Cantu Alanis
// Class for Genetic Algorithm
// 2-29-2016

#include <iostream>
#include <string>

using namespace std;

template <class T>
class GA{
  int populationSize;
  T * population;
public:
  GA(int n){
    if(n > 10){
      populationSize = n*10;
    }
    else{
      cout<<"Error. You need more than 10.";
      return;
    }
    population = new T[populationSize];
    for(int i = 0; i<populationSize; i++)
      population[i].generateIndividual(n);
  }
  void parentSelection(){
    return;
  }
  void mutation(){
    return;
  }
  void recombination(){
    return;
  }
  void printPopulation(){
    for(int i = 0; i<populationSize; i++)
      population[i].printGenotype();
  }
};
