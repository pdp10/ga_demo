/*
 * Population.h
 * Copyright (C) Piero Dalle Pezze 2007 <piero.dallepezze@gmail.com>
 * 
 * geneticalgorithm is free software.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 

#ifndef POPULATION_H
#define POPULATION_H


#include <vector>
#include <iostream>
#include "Citizen.h"
#include <math.h>
extern "C" {
#include "mt19937ar.h"
}
using namespace std;


/** This class represents a population of generic citizens. */
class Population {

 protected:

  /////////////////////////
  ////// DATA FIELDS //////
  /////////////////////////

  // PARAMETERS
  /** The size of the population. */
  int populationSize;
  /** The percentage of population to crossover. */
  float crossoverRate;
  /** The percentage of population to mutate. */
  float mutationRate;

  // ALGORITHMS
  /** The choice of the selection algorithm. */
  int selectionChoice;
  /** The choice of the crossover algorithm. */
  int crossoverChoice;
  /** The choice of the random generator algorithm. */
  int randomGeneratorChoice;
  /** The choice of the probabilistic algorithm. */
  int probabilisticChoice;
  /** The choice of the fitness algorithm. */
  int fitnessChoice;

  // THE POPULATION
  /** The population of citizens. */
  vector<Citizen*> population;
  /** The probabilities of citizens. */
  vector<double> populationProbabilities;
  /** The distribution function. It is related 
   * with the populationProbabilities vector. */
  vector<double> distributionFunction;
  /** The best citizen of the population. */
  Citizen* bestCitizen;
  /** The average fitness of the population. */
  double fitnessAV;

  /////////////////////////
  //////// METHODS ////////
  /////////////////////////

  // SORTING
  /** It sorts the vector _citizen. It uses the < operator 
   * defined on the class Citizen. */
  void sortByFitness(vector<Citizen*>& _citizens);

  // PROBABILITY
  /** It computes the density function on the range [_start, _end) 
   * of the population. Pr(h_i) = fitness(h_i) / sum_i(fitness(h_i)) .*/
  virtual void computeDensity(const int& _start, const int& _end);
  /** It computes the distribution function using the 
   * populationProbabilities vector. */
  virtual void computeDistributionFunction();
  /** It chooses at random the index of a citizen. _extracted is
   * the number of citizens chosen. */
  virtual int makeProbabilisticChoice(const int& _extracted) const;

  // SELECTION ALGORITHMS
  /** It selects the best citizens and adds them to the _nextGeneration. */
  virtual void elitism(Population* _nextGeneration) = 0;
  /** It probabilistically selects the best citizens and adds them to the 
   * _nextGeneration. In this algorithm, the probabilistic choice is BY_FITNESS. */
  virtual void rouletteWheelSelection(Population* _nextGeneration) = 0;
  /** It probabilistically selects the best citizens and adds them to the _nextGeneration. 
   * On details it selects pairs of citizens, and compares their fitness. The best citizen 
   * is chosen. */
  virtual void tournamentSelection(Population* _nextGeneration) = 0;
  /** It probabilistically selects the best citizens and adds them to the _nextGeneration. 
   * On details, it ranks the population by fitness.
   * From the worst to the best citizen, ranks are 1 3 6 10 15 21 28 ... (rank(n-1)+n). 
   * Then it works as the rouletteWheelSelection by using ranking instead of probabilities. */
  virtual void rankSelection(Population* _nextGeneration) = 0;

  // CROSSOVER ALGORITHMS
  /** For each pair of citizens, it chooses at random one point of their representation 
   * and makes the crossover, breeding two new offsprings. */
  virtual void singlePointCrossover(Population* _nextGeneration) = 0;
  /** For each pair of citizens, it chooses at random two points of their representation 
   * and makes the crossover, breeding two new offsprings. */
  virtual void twoPointCrossover(Population* _nextGeneration) = 0;
  /** For each pair of citizens, it chooses at random some points of their representation 
   * and makes the crossover only on those points, breeding two new offsprings. */
  virtual void uniformCrossover(Population* _nextGeneration) = 0;

  // MUTATION ALGORITHMS
  /** It mutates the representation of _citizen on _position of _delta. */
  virtual void mutate(Citizen* _citizen,
		      const int& _position,
		      const int& _delta) = 0;

  // FITNESS ALGORITHMS
  /** For each citizen, it computes its fitness using a linear formula. */
  virtual void linearFitness() = 0;
  /** For each citizen, it computes its fitness using a squared formula. */
  virtual void squareFitness() = 0;

  // AUXILIAR
  /** It swaps the citizens of indexes _first and _second. */
  void swap(const int& _first, const int& _second);

 public:
  /** It constructs an empty population of citizens.s
   * By default it uses ELITISM, SINGLE_POINT_CROSSOVER, RAN0,
   * UNIFORM, LINEAR as algorithms. */
  Population(const float& _crossoverRate = 0.90f, 
	     const float& _mutationRate  = 0.25f);
  /** It destructs the population by invoking for each citizen
   * its destructor. */
  virtual ~Population();
  /** It initializes the size of the population. You must override this
   * method to initializes the population with your citizens. */
  virtual void init(const int& _populationSize);

  // METHODS INVOKED BY THE MAIN GENETIC ALGORITHM
  /** It selects populationSize * (1 - crossoverRate) citizens
   * and adds them to the _nextGeneration. */
  virtual void selection(Population* _nextGeneration);
  /** It makes the crossover on (populationSize * crossoverRate)/2 pairs of citizens
   * and adds their offsprings to the _nextGeneration. */
  virtual void crossover(Population* _nextGeneration);
  /** It mutates populationSize * (mutationRate) citizens of the population. */ 
  virtual void mutation() = 0;
  /** For each citizen, it computes its fitness. */
  virtual void computeFitness();
  /** It adds _citizen to this population. */
  void addCitizen(Citizen* _citizen);
  /** It prints the population. */
  virtual void print();
	
  // TO CONFIGURE THE SELECTION
  /** Elitism as selection algorithm. */
  static const int ELITISM = 0;
  /** Roulette Wheel Selection as selection algorithm. */
  static const int ROULETTE_WHEEL_SELECTION = 1;
  /** Tournament Selection as selection algorithm. */
  static const int TOURNAMENT_SELECTION = 2;
  /** Rank Selection as selection algorithm. */
  static const int RANK_SELECTION = 3;

  // TO CONFIGURE THE CROSSOVER
  /** Single Point Crossover as crossover algorithm. */
  static const int SINGLE_POINT_CROSSOVER = 0;
  /** Two Point Crossover as crossover algorithm. */
  static const int TWO_POINT_CROSSOVER = 1;
  /** Uniform Crossover as crossover algorithm. */
  static const int UNIFORM_CROSSOVER = 2;

  // TO CONFIGURE THE RANDOM GENERATOR
  /** Ran0 (C/C++ rand() function) as random generator algorithm. */
  static const int RAN0 = 0;
  /** Mersenne Twister (modified) as random generator algorithm. */
  static const int MT19937AR = 1;

  // TO CONFIGURE THE PROBABILISTIC CHOICE
  /** It chooses with uniform probability. */
  static const int UNIFORM = 0;
  /** It chooses with probability defined on citizens' fitness. */
  static const int BY_FITNESS = 1;

  // TO CONFIGURE THE FITNESS FUNCTION
  /** It computes fitness of citizens using a linear formula. */
  static const int LINEAR = 0;
  /** It computes fitness of citizens using a squared formula. */
  static const int SQUARE = 1;

  // GET and SET

  /** It returns the best citizen of the population.
   * It is the citizen which best fits the target. */
  Citizen* getBest() const;  
  /** It returns the vector of citizens. */
  vector<Citizen*> getCitizens() const;
  /** It returns the average fitness of the population. */
  double getFitnessAV() const;

  /** It returns the selection choice. */
  int getSelectionChoice() const;
  /** It returns the crossover choice. */
  int getCrossoverChoice() const;
  /** It returns the random generator choice. */
  int getRandomGeneratorChoice() const;
  /** It returns the probabilistic choice. */
  int getProbabilisticChoice() const;
  /** It returns the fitness choice. */
  int getFitnessChoice() const;

  /** It sets the selection algorithm. */
  void setSelectionChoice(const int& _selection);
  /** It sets the crossover algorithm. */
  void setCrossoverChoice(const int& _crossover);
  /** It sets the random generator algorithm. */
  void setRandomGeneratorChoice(const int& _generator);
  /** It sets the probabilistic choice. */
  void setProbabilisticChoice(const int& _probabilisticChoose);
  /** It sets the fitness algorithm. */
  void setFitnessChoice(const int& _fitnessFunction);

}; // end class Population

#endif
