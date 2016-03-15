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
      populationSize = n;
    }
    population = new T[populationSize];
  }
};
