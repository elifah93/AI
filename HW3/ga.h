// Elifaleth Cantu Alanis
// Class for Genetic Algorithm
// 2-29-2016

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

/* Class for Geneteic Algorithm.
  It uses template, so we can use it with different problems. */
template <class T>
class GA{
  int populationSize;
  int mantingPoolSize;
  int newGenerationSize;
  int newGenerationCount;
  // Pool where we store the index of the individuals select in parentSelection()
  int * matingPool;
  // Where the population is going to be stored.
  T * population;
  // Where the new generation is going to be stored.
  T * newGeneration;
public:
  /* Initialize the population
  Parameters: n, the size for N-queens.*/
  GA(int n){
    // n should not be greater or equal to 100.
    if(n>=100){
      cout<<"Sorry you cannot use n greater than 100"<<endl;
      return;
    }
    // n should be at least 10
    if(n >= 10){
      populationSize = n*10; // The size of the population will be n * 10.
    }
    else{
      cout<<"Error. You need more than 10.";
      return;
    }
    population = new T[populationSize];
    // Generate or create every individual in the population.
    for(int i = 0; i<populationSize; i++){
      population[i].generateIndividual(n);
    }
  }
  /* It will get 3 individuals from the population, and it will
  select the one with the best fitness. The individuals with better fitness
  will be stored in the matingPool.*/
  void parentSelection(){
    int current = 0;
    double bestFitness = 0.0;
    int bestCompetitor;
    int randomCompe;
    //The size of the matingPool is going to be the 10% of the populationSize
    mantingPoolSize = populationSize*0.10;
    if(!(mantingPoolSize%2==0))// Make sure the matingPoolSize is always even.
      mantingPoolSize++;
    matingPool = new int[mantingPoolSize];
    newGenerationSize = mantingPoolSize;
    newGeneration = new T[newGenerationSize];
    newGenerationCount=0;

    // Tournament between individuals. Only the ones with good fitness will be selected.
    while(current < mantingPoolSize){
      for(int i=0;i<3;i++){
        randomCompe = rand()%populationSize;
        if(population[randomCompe].getFitness()>bestFitness){
          bestCompetitor = randomCompe;
          bestFitness = population[randomCompe].getFitness();
        }
      }
      matingPool[current] = bestCompetitor;
      bestFitness=0.0;
      bestCompetitor = -1;
      current++;
    }
  }
  /* It will mutate an individual depending on the result of a random number
  between 0 and 1. If the number is less than 0.10 it will mutate, by choosing
   two genes of the individual and swap them. */
  void mutation(){
    int s1,s2;
    int swapGene1,swapGene2;
    for(int i=0; i<newGenerationCount-1;i++){
      if(((double) rand() / (RAND_MAX))<=0.10){
        s1=rand()%newGeneration[i].getN();;
        swapGene1=newGeneration[i].getGene(s1);
        s2=rand()%newGeneration[i].getN();
        swapGene2=newGeneration[i].getGene(s2);
        swap(swapGene1,swapGene2);
        newGeneration[i].alteringGene(swapGene1,s1);
        newGeneration[i].alteringGene(swapGene2,s2);
      }
    }
  }
  /* It will grab two individuals from matingPool, and combine them.
  The method I am using it is very simple, it takes the ith individual and the next individual.
   */
  void crossover(){
    for(int i=0;i<mantingPoolSize;i++){
      recombination(matingPool[i],matingPool[i+1]);
      i++;
    }
  }
  /* Combines 2 parents to create 2 children for the next generation.
  Parameters: p1, index for parent 1.
              p2, index for parent 2.*/
  void recombination(int p1,int p2){
    int gene;
    int i;
    int index;
    int coPoint=rand()%population[0].getN(); // Crossover point.
    // Create individual, and set all gene values to 0.
    newGeneration[newGenerationCount].generateIndividual(population[p1].getN(),true);
    newGeneration[newGenerationCount+1].generateIndividual(population[p1].getN(),true);

    // Copy the first part of the parents to the children.
    for(int i=0;i<coPoint;i++){
      newGeneration[newGenerationCount].alteringGene(population[p1].getGene(i),i);
      newGeneration[newGenerationCount+1].alteringGene(population[p2].getGene(i),i);
    }

    gene=-1;
    index=coPoint;
    i=0;
    // Copies the genes that are not in the children from parent2
    while(true){
      if(i>=population[p2].getN()|| index>=population[p2].getN()){
        break;
      }
      gene = population[p2].getGene(i);
      if(!newGeneration[newGenerationCount].findGene(gene)){
        newGeneration[newGenerationCount].alteringGene(gene,index);
        index++;
      }
      i++;
    }
    // If the number of genes that has been changed is less than n. It is going
    // to add missing numbers sequentially.
    if(index<newGeneration[newGenerationCount].getN()){
      for(int k=0;k<newGeneration[newGenerationCount].getN();k++){
        if(index>=population[p2].getN()){
          break;
        }
        if(!newGeneration[newGenerationCount].findGene(k+1)){
            newGeneration[newGenerationCount].alteringGene(k+1,index);
            index++;
        }
      }
    }
    // Copies the genes that are not in the children from parent2
    gene=-1;
    i=0;
    index=coPoint;
    while(true){
      if(i>=population[p1].getN()|| index>=population[p1].getN()){
        break;
      }
      gene = population[p1].getGene(i);
      if(!newGeneration[newGenerationCount+1].findGene(gene)){
        newGeneration[newGenerationCount+1].alteringGene(gene,index);
        index++;
      }
      i++;
    }
    // If the number of genes that has been changed is less than n. It is going
    // to add missing numbers sequentially.
    if(index<newGeneration[newGenerationCount+1].getN()){
      for(int k=0;k<newGeneration[newGenerationCount+1].getN();k++){
        if(index>=population[p1].getN()){
          break;
        }
        if(!newGeneration[newGenerationCount+1].findGene(k+1)){
            newGeneration[newGenerationCount+1].alteringGene(k+1,index);
            index++;
        }
      }
    }
    newGenerationCount += 2;
  }
  /* It is going to replace the parent with the children. It is not best way to do it.
  but it is fast. Age-biased.*/
  void survivorSelection(){
    for(int i=0;i<mantingPoolSize;i++){
      population[matingPool[i]]=newGeneration[i];
    }
    delete[] matingPool;
    delete[] newGeneration;
  }
  /* Evaluate the fitness of every individual. */
  void fitnessEvalutation(){
    for(int i = 0; i<populationSize; i++){
      population[i].calculateFitness();
    }
  }
  /* Print the genotype of every individual to the screen. */
  void printPopulation(){
    for(int i = 0; i<populationSize; i++){
      population[i].printGenotype();
    }
  }
  /* Evaluate the fitness of the new generation. */
  void fitnessEvalutationChildren(){
    for(int i = 0; i<newGenerationCount; i++){
      newGeneration[i].calculateFitness();
    }
  }
  /* Print to the screen the new generation. */
  void printChildren(){
    for(int i = 0; i<newGenerationCount; i++){
      newGeneration[i].printGenotype();
    }
  }
  /* Return where there is a solution in the population or not. */
  bool foundSolution(fstream& f){
    for(int i=0;i<populationSize;i++){
      if(population[i].getFitness() == 10000){
        f<<i<<" ";
        population[i].printGenotypeToFile(f);
        cout<<endl;
        return true;
      }
    }
    return false;
  }
  /* Find the individual with the highest fitness. */
  void getHighestFitness(){
    int index=-1000;
    int highest = -1000;
    for(int i=0;i<populationSize;i++){
      if(population[i].getFitness()>highest){
        highest=population[i].getFitness();
        index=i;
      }
    }
    population[index].printGenotype();
    cout<<"fitness: "<<population[index].getFitness()<<endl;
  }
  /* Delete the population. */
  void termination(){
    delete[] population;
  }
};
