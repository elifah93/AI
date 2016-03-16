// Elifaleth Cantu
// Class for N Queens
// 2-29-2016

#include <algorithm>
#include <vector>

class Nqueen{
  int n;
  vector<int> fakeGenotype;
  int *genotype;

public:
  void generateIndividual(int size){
    int i;
    n = size;
    for (int i=1; i<=n; ++i) fakeGenotype.push_back(i);
    random_shuffle ( fakeGenotype.begin(), fakeGenotype.end() );
    genotype = new int[n];
    i = 0;
    for (std::vector<int>::iterator it=fakeGenotype.begin(); it!=fakeGenotype.end(); ++it){
      genotype[i]=*it;
      i++;
    }
  }
  void printGenotype(){
    for (int i=0; i<n; i++)
      cout<<genotype[i]<<" ";
    cout<<endl;
  }
  int conflicts(){
    // Check for the conflicts
    return 0;
  }
};
