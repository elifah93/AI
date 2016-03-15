// Elifaleth Cantu
// Class for N Queens
// 2-29-2016

#include <algorithm>
#include <vector>

class Nqueen{
  int n;
  vector<int> genotype;

public:
  void generateIndividual(int size){
    n = size;
    for (int i=1; i<=n; ++i) genotype.push_back(i);
    random_shuffle ( genotype.begin(), genotype.end() );
  }
  void printGenotype(){
    for (std::vector<int>::iterator it=genotype.begin(); it!=genotype.end(); ++it)
      cout << ' ' << *it;
  }
};
