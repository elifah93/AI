// Elifaleth Cantu
// Class for N Queens
// 2-29-2016

/* Class for Nqueen problem */
class Nqueen{
  int n;
  int conf; // Number of conflicts.
  int *genotype;
  double fitness;

public:
  /* Creates the genotype of an individual.
  Parameters: size, will be n for Nqueen.
              empty, is whether we want to generate random genes for the individual,
              or leave it in zeros.*/
  void generateIndividual(int size,bool empty=false){
    n = size;
    conf=0;
    genotype = new int[n];

    if(!empty){
      for(int i=0;i<n;i++){
        genotype[i]=rand() % n + 1;
      }
    }
    else{
      for(int i=0;i<n;i++){
        genotype[i]=0;
      }
    }
  }
  /* Display the genotype to the screen. */
  void printGenotype(){
    cout<<"< ";
    for (int i=0; i<n; i++)
      cout<<genotype[i]<<" ";
    cout<<"> ";
  }
  /* Prints the genotype to a file */
  void printGenotypeToFile(fstream &f){
    f<<"<";
    for (int i=0; i<n; i++){
      if(i != 0)      
        f<<","<<genotype[i];
      else
        f<<genotype[i];
    }
    f<<">";
  }
  /* Gets the number of conflicts for the individual.
  The way it works is that from index i, it will check only if there are conflicts to the right.
  it will check if there is an horizontal conflict, a conflict going diagonal up, and a conflict
  going diagonal down.*/
  void conflicts(){
    conf=0;
    bool d1 = false;
    bool d2 = false;
    bool v = false;
    for(int i=0;i<n;i++){
      for(int j=i+1;j<n;j++){
        // Checks for horizontal conflict.
        if(!v && genotype[i]==genotype[j]){
          conf++;
          v=true;
        }
        /* Checks for diagonal conflicts.
         If the difference between i and j is the same that the values of
         genotype[i] and genotype[j], that means that is forms an square,
         so they have conflict. */
        if(abs(genotype[i]-genotype[j]) == abs(i-j)){
          if(!d1 && genotype[i]>genotype[j]){
            conf++;
            d1=true;
          }
          else if(!d2 && genotype[i]<genotype[j]){
            conf++;
            d2=true;
          }
        }
      }
      d1=false;
      d2=false;
      v=false;
    }
  }
  /* Gets the conflicts and calculate fitness. */
  void calculateFitness(){
      conflicts();
      fitness = 1/(conf+0.0001);
  }
  /* Return the fitness. */
  double getFitness(){
    return fitness;
  }
  /* Return n. */
  int getN(){
    return n;
  }
  /* Return the values of the gene at the specified index.
  Parameters: index, index of the gene in the genotype.*/
  int getGene(int index){
    return genotype[index];
  }
  /* Alter a gene.
  Parameters: gene, new gene value.
              index, index of the gene that is going to be alter.*/
  void alteringGene(int gene, int index){
    genotype[index]=gene;
  }
  /* Return where there is an specific gene in the genotype of not. 
  Parameters: gene, value of the gene we want to find.*/
  bool findGene(int gene){
    for(int i=0;i<n;i++){
      if(genotype[i]==gene)
        return true;
    }
    return false;
  }
  /* Return the number of conflicts. */
  int getConflicts(){
    return conf;
  }
};
