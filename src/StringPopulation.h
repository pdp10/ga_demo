/*
 * StringPopulation.h
 * Copyright (C) Piero Dalle Pezze 2007 <->
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


#ifndef STRING_POPULATION_H
#define STRING_POPULATION_H


#include "Population.h"
#include "StringCitizen.h"
#include <string>
using namespace std;


/** This class represents a population whose citizens are 
 * objects of type StringCitizen. */ 
class StringPopulation : public Population {

 private:
  /** The target to find. */
  string target;

 protected:

  // SELECTION ALGORITHMS
  /** It selects the best citizens and adds them to the _nextGeneration. */
  virtual void elitism(Population* _nextGeneration);
  /** It probabilistically selects the best citizens and adds them to the 
   * _nextGeneration. In this algorithm, the probabilistic choice is BY_FITNESS. */
  virtual void rouletteWheelSelection(Population* _nextGeneration);
  /** It probabilistically selects the best citizens and adds them to the _nextGeneration. 
   * On details it selects pairs of citizens, and compares their fitness. The best citizen 
   * is chosen. */
  virtual void tournamentSelection(Population* _nextGeneration);
  /** It probabilistically selects the best citizens and adds them to the _nextGeneration. 
   * On details, it ranks the population by fitness.
   * From the worst to the best citizen, ranks are 1 3 6 10 15 21 28 ... (rank(n-1)+n). s
   * Then it works as the rouletteWheelSelection by using ranking instead of probabilities. */
  virtual void rankSelection(Population* _nextGeneration);

  // CROSSOVER ALGORITHMS
  /** For each pair of citizens, it chooses at random one point of their representation 
   * and makes the crossover, breeding two new offsprings. */
  virtual void singlePointCrossover(Population* _nextGeneration);
  /** For each pair of citizens, it chooses at random two points of their representation 
   * and makes the crossover, breeding two new offsprings. */
  virtual void twoPointCrossover(Population* _nextGeneration);
  /** For each pair of citizens, it chooses at random some points of their representation 
   * and makes the crossover only on those points, breeding two new offsprings. */
  virtual void uniformCrossover(Population* _nextGeneration);

  // MUTATION ALGORITHMS
  /** It mutates the representation of _citizen on _position of _delta. */
  virtual void mutate(Citizen* _citizen, 
		      const int& _position,
		      const int& _delta);

  // FITNESS ALGORITHMS
  /** For each citizen, it computes its fitness using a linear formula. 
   * The fitness is computed as the distance between the representation of the i-th citizen and
   * the target using the lexicographic order. */
  virtual void linearFitness();
  /** For each citizen, it computes its fitness using a squared formula.
   * The fitness is computed as the distance between the representation of the i-th citizen and
   * the target using the lexicographic order. */ 
 virtual void squareFitness();

 public:
  /** It constructs an empty population of citizens.
   * By default it uses ELITISM, SINGLE_POINT_CROSSOVER, RAN0,
   * UNIFORM, LINEAR as algorithms. */
  StringPopulation(const string& _target       = "", 
		   const float& _crossoverRate = 0.90f, 
		   const float& _mutationRate  = 0.25f);
  /** It destructs the population by invoking for each citizen
   * its destructor. */
  virtual ~StringPopulation();
  /** It initializes _populationSize using objects of type StringCitizen each one with a random string. */
  virtual void init(const int& _populationSize);
  /** It mutates populationSize * (mutationRate) citizens of the population. */ 
  virtual void mutation();

}; // end class StringPopulation

#endif
