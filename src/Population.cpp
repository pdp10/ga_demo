/*
 * Population.cpp
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

 
#include <algorithm>
#include "Population.h"



/** It returns true if _first has a fitness lesser than the _second one. */
inline bool fitnessSort(Citizen* _first, Citizen* _second) {
  return _first->getFitness() < _second->getFitness();		
}



/************************************** 
 **** CONSTRUCTORS and DESTRUCTORS ****
 **************************************/

Population::Population(const float& _crossoverRate, 
		       const float& _mutationRate) 
  : bestCitizen(0), 
    populationSize(0), 
    selectionChoice(Population::ELITISM),
    crossoverChoice(Population::SINGLE_POINT_CROSSOVER),
    randomGeneratorChoice(Population::RAN0),
    probabilisticChoice(Population::UNIFORM),
    fitnessChoice(Population::LINEAR) {
  // controls and instatiations
  if(_crossoverRate < 0.0f || _crossoverRate >= 1.0) {
    cerr << "\nError in constructor: \n" <<
      "\tPopulation (const float& _crossoverRate,\n" << 
      "\t            const float& _mutationRate,\n" <<
      "\t            const float& _elitismRate)\n" <<
      "\t_crossoverRate must be [0.0f..1.0f)\n" <<
      "\tDefault instatiation: _crossoverRate = 0.90f\n";
    crossoverRate = 0.90f; 
  } else { 
    crossoverRate = _crossoverRate; 
  }
  if(_mutationRate < 0.0f || _mutationRate >= 1.0) {
    cerr << "\nError in constructor: \n" <<
      "\tPopulation (const float& _crossoverRate,\n" << 
      "\t            const float& _mutationRate,\n" <<
      "\t            const float& _elitismRate)\n" <<
      "\t_mutationRate must be [0.0f..1.0f)\n" <<
      "\tDefault instatiation: _mutationRate = 0.25f\n";
    mutationRate = 0.25f; 
  } else { 
    mutationRate = _mutationRate; 
  }
  population = vector<Citizen*>();
}

Population::~Population() {
  for(int i = 0; i < populationSize; i++) {
    /* It inkokes the destructor for each citizen. */
    delete population[i];
  }
}

void Population::init(const int& _populationSize) {
  for(int i = 0; i < populationSize; i++) {
    delete population[i];
  }
  if(_populationSize <= 0) {
    cerr << "\nError in method: \n" << 
      "\tvoid Population::init (const int& _populationSize)\n" <<
      "\t_populationSize must be in [1..n]\n" <<
      "\tDefault instatiation: _populationSize = 100\n";
    populationSize = 100;
  } else { 
    populationSize = _populationSize; 
  }
}



/************************************** 
 ************** SORTING ***************
 **************************************/

void Population::sortByFitness(vector<Citizen*>& _citizen) {
  sort(_citizen.begin(), _citizen.end(), fitnessSort);
}



/************************************** 
 ************ PROBABILITY *************
 **************************************/

void Population::computeDensity(const int& _start, const int& _end) {
  populationProbabilities = vector<double>();
  /* It computes the total fitness. */
  int totalFitness = 0;
  for(int i = _start; i < _end; i++) {
    totalFitness = totalFitness + population[i]->getFitness();
  }
  /* It computes the probability for each citizen. */
  for(int i = _start; i < _end; i++) {
    populationProbabilities.push_back((double)population[i]->getFitness() / (double)totalFitness);
    //cout << "populationProbabilities["<<i<<"]="<<populationProbabilities[i] << endl;
  }
}

void Population::computeDistributionFunction() {
  distributionFunction = vector<double>();
  /* It computes the monotone increasing distribution function. */
  if(populationProbabilities.size() > 0) {
    distributionFunction.push_back(populationProbabilities[0]);
  }
  for(int i = 1; i < populationProbabilities.size(); i++) {
    distributionFunction.push_back(distributionFunction[i-1] + populationProbabilities[i]);
    //cout << "distributionFunction["<<i<<"]="<<distributionFunction[i] << endl;
  }
}

int Population::makeProbabilisticChoice(const int& _extracted) const {
  double rnd = 0.0;
  int citizenIdx = 0;
  bool stop = false;
  switch(probabilisticChoice) {
  case UNIFORM:
    /* It chooses a citizen in the range [_extracted, populationSize). */
    switch(randomGeneratorChoice) {
    case RAN0 :
      /* It uses the common rand() function. */
      citizenIdx = (rand() % (populationSize - _extracted) + _extracted);
      break;
    case MT19937AR :
      /* It uses the Mersenne Twister to generate random numbers. */
      citizenIdx = (genrand_int32() % (populationSize - _extracted)) + _extracted;
      break;
    }
    break;
  case BY_FITNESS:
    /* It extracts a double in [0, MAX_VALUE). */
    switch(randomGeneratorChoice) {
    case RAN0 :
      /* It uses the common rand() function. */
      rnd = (double)rand() / (double)RAND_MAX;
      break;
    case MT19937AR :
      /* It uses the Mersenne Twister to generate random numbers. */
      rnd = (double)genrand_int32() / (double)0xffffffff;
      break;
    }
    /* Remember that the size of the distributionFunction vector is
     * populationSize - _extracted. The distributionFunction is related
     * only with populationSize - _extracted citizen not already extracted. */
    for(int i = 0; i < distributionFunction.size() && !stop; i++) {
      if(rnd < distributionFunction[i]) {
	/* Shifting/ */
	citizenIdx = i + _extracted;
	stop = true;
      }
    }
    break;
  }
  return citizenIdx;
}



/*************************************** 
 ************ MAIN METHODS *************
 ***************************************/

void Population::selection(Population* _nextGeneration) {
  switch(selectionChoice) {
  case ELITISM: 
    elitism(_nextGeneration); 
    break;
  case ROULETTE_WHEEL_SELECTION: 
    rouletteWheelSelection(_nextGeneration); 
    break;
  case TOURNAMENT_SELECTION: 
    tournamentSelection(_nextGeneration); 
    break;
  case RANK_SELECTION: 
    rankSelection(_nextGeneration); 
    break;
  }
}

void Population::crossover(Population* _nextGeneration) {
  switch(crossoverChoice) {
  case SINGLE_POINT_CROSSOVER: 
    singlePointCrossover(_nextGeneration); 
    break;
  case TWO_POINT_CROSSOVER: 
    twoPointCrossover(_nextGeneration); 
    break;
  case UNIFORM_CROSSOVER: 
    uniformCrossover(_nextGeneration); 
    break;
  }
}

void Population::computeFitness() {
  switch(fitnessChoice) {
  case LINEAR:
    linearFitness();
    break;
  case SQUARE:
    squareFitness();
    break;
  }
  switch(probabilisticChoice) {
  case UNIFORM:
    break;
  case BY_FITNESS:
    computeDensity(0, populationSize);
    computeDistributionFunction();
    break;
  }
}



/*************************************** 
 ************** AUXILIAR ***************
 ***************************************/

void Population::swap(const int& _first, const int& _second) {
  Citizen* temp       = population[_first];
  population[_first]  = population[_second];
  population[_second] = temp;
}

void Population::addCitizen(Citizen* _citizen) {
  populationSize++;
  population.push_back(_citizen);
}


/*************************************** 
 ************* GET and SET *************
 ***************************************/

Citizen* Population::getBest() const { return bestCitizen; }

vector<Citizen*> Population::getCitizens() const { return population; }

double Population::getFitnessAV() const { return fitnessAV; }

int Population::getSelectionChoice() const { return selectionChoice; }

int Population::getCrossoverChoice() const { return crossoverChoice; }

int Population::getRandomGeneratorChoice() const { return randomGeneratorChoice; }

int Population::getProbabilisticChoice() const { return probabilisticChoice; }

int Population::getFitnessChoice() const { return fitnessChoice; }

void Population::setSelectionChoice(const int& _selection) {
  if(_selection < 0 || _selection > 3) {
    cerr << "\nError in method: \n" << 
      "\tPopulation::setSelectionChoice(const int& _selection)\n" <<
      "\t_selection must be in [0..3]\n" <<
      "\tDefault instatiation: selectionChoice = Population::ELITISM\n";
    selectionChoice = Population::ELITISM;
  } else {
    selectionChoice = _selection;	
  }
}

void Population::setCrossoverChoice(const int& _crossover) {
  if(_crossover < 0 || _crossover > 2) {
    cerr << "\nError in method: \n" << 
      "\tPopulation::setCrossoverChoice(const int& _crossover)\n" <<
      "\t_crossover must be in [0..3]\n" <<
      "\tDefault instatiation: crossoverChoice = Population::SINGLE_POINT_CROSSOVER\n";
    crossoverChoice = Population::SINGLE_POINT_CROSSOVER;
  } else {
    crossoverChoice = _crossover;	
  }
}

void Population::setRandomGeneratorChoice(const int& _generator) {
  if(_generator < 0 || _generator > 1) {
    cerr << "\nError in method: \n" << 
      "\tPopulation::setGeneratorChoice(const int& _generator)\n" <<
      "\t_generator must be in [0..1]\n" <<
      "\tDefault instatiation: generatorChoice = Population::RAN0\n";
    randomGeneratorChoice = Population::RAN0;
  } else {
    randomGeneratorChoice = _generator;	
  }
}

void Population::setProbabilisticChoice(const int& _probabilisticChoice) {
  if(_probabilisticChoice < 0 || _probabilisticChoice > 1) {
    cerr << "\nError in method: \n" << 
      "\tPopulation::setProbabilisticChoice(const int& _probabilisticChoice)\n" <<
      "\t_probabilisticChoice must be in [0..1]\n" <<
      "\tDefault instatiation: probabilisticChoice = Population::UNIFORM\n";
    probabilisticChoice = Population::UNIFORM;
  } else {
    probabilisticChoice = _probabilisticChoice;	
  }
}

void Population::setFitnessChoice(const int& _fitnessFunction) {
  if(_fitnessFunction < 0 || _fitnessFunction > 1) {
    cerr << "\nError in method: \n" << 
      "\tPopulation::setFitnessChoice(const int& _fitnessFunction)\n" <<
      "\t_fitnessFunctionmust be in [0..1]\n" <<
      "\tDefault instatiation: fitnessChoice = Population::LINEAR\n";
    fitnessChoice = Population::LINEAR;
  } else {
    fitnessChoice = _fitnessFunction;	
  }
}



/*************************************** 
 ************** PRINTING ***************
 ***************************************/

void Population::print() {
  cout << "POPULATION:\n";
  cout << "Average fitness: " << fitnessAV << endl;
  for(int i = 0; i < populationSize; i++) {
    population[i]->print();
    cout << endl;
  }
}
