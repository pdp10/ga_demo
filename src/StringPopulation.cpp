/*
 * StringPopulation.cpp
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

  
#include "StringPopulation.h"
#include <deque>
#include <cstdlib>



/************************************** 
 **** CONSTRUCTORS and DESTRUCTORS ****
 **************************************/

StringPopulation::StringPopulation(const string& _target, 
				   const float& _crossoverRate, 
				   const float& _mutationRate) 
  : Population(_crossoverRate, _mutationRate), target(_target) { }

StringPopulation::~StringPopulation() { }

void StringPopulation::init(const int& _populationSize) {
  Population::init(_populationSize);
  string temp = "";
  /* It initializes a population of objects of type StringCitizen. */
  for(int i = 0; i < populationSize; i++) {
    StringCitizen* citizen = new StringCitizen(); 
    temp.erase();
    /* It generates a string representation for the i-th citizen. */
    for(int j = 0; j < target.size(); j++) {
      switch(randomGeneratorChoice) {
	/* The alphabet consists of 90 symbols. */
      case RAN0 :      temp += (rand() % 90) + 32;          break;
      case MT19937AR : temp += (genrand_int32() % 90) + 32; break;
      }
    }
    citizen->setString(temp);
    population.push_back(citizen);
  }
}



/***********************************
*********** SELECTION **************
************************************/ 

void StringPopulation::elitism(Population* _nextGeneration) {
  StringCitizen* citizen = 0;
  int elitismSize = (int) (populationSize * (1.0f - crossoverRate));
  /* It copy the population and sort the copied vector by fitness increasing. */
  vector<Citizen*> _citizens = population;
  sortByFitness(_citizens);
  /* It extracts elitismSize citizens which are the best ones and 
   * adds them to the _nextGeneration. */
  for(int i = 0; i < elitismSize; i++) {
    citizen = new StringCitizen();
    citizen->setString((dynamic_cast<StringCitizen*>(_citizens[i]))->getString());
    citizen->setFitness(_citizens[i]->getFitness());
    _nextGeneration->addCitizen(citizen);
  }
}

void StringPopulation::rouletteWheelSelection(Population* _nextGeneration) {
  int selectionSize = (int) (populationSize * (1.0f - crossoverRate));
  string str = "";
  double rnd = 0.0;
  int citizenIdx = 0;
  StringCitizen* citizen = 0;
  bool stop = false;
  int selection = probabilisticChoice;
  // the best is added
  /* It adds the best citizen to the _nextGeneration. In this way, it prevents
   * to lose the best solution found until now. */
  str = (dynamic_cast<StringCitizen*>(bestCitizen))->getString();
  citizen = new StringCitizen(str);
  _nextGeneration->addCitizen(citizen);
  /* It probabilistically chooses other selectionSize - 1 citizens to the _nextGeneration
   * choosing them considering their fitness. */
  for(int i = 0; i < selectionSize - 1; i++) {
    probabilisticChoice = BY_FITNESS;
    citizenIdx = makeProbabilisticChoice(i);
    str = (dynamic_cast<StringCitizen*>(population[citizenIdx]))->getString();
    citizen = new StringCitizen(str);
    _nextGeneration->addCitizen(citizen);
    /* It swaps the two indexes. */
    swap(i, citizenIdx);
    /* It re-computes the density of citizens which are not extracted yet. */
    computeDensity(i+1, populationSize);
    /* It re-computes, for those citizens, the distribution function. */ 
    computeDistributionFunction();
  }
  probabilisticChoice = selection;
  switch(probabilisticChoice) {
  case UNIFORM:
    break;
  case BY_FITNESS:
    /* It restores the density and the distribution function. */
    computeDensity(0, populationSize);
    computeDistributionFunction();
    break;
  }
}

void StringPopulation::tournamentSelection(Population* _nextGeneration) {
  int selectionSize = (int) (populationSize * (1.0f - crossoverRate));
  string str = "";
  StringCitizen* winner = 0;
  int winnerIdx = 0;
  int i1 = 0, i2 = 0;
  for(int i = 0; i < selectionSize; i++) {
    /* It chooses two citizens on the range [i, populationSize]. */
    i1 = makeProbabilisticChoice(i);
    i2 = makeProbabilisticChoice(i);
    if(population[i1]->getFitness() <= population[i2]->getFitness()) {
      str = (dynamic_cast<StringCitizen*>(population[i1]))->getString();
      winnerIdx = i1;
    } else {
      str = (dynamic_cast<StringCitizen*>(population[i2]))->getString();
      winnerIdx = i2;
    }
    winner = new StringCitizen(str);
    _nextGeneration->addCitizen(winner);
    /* It swaps the two indexes. */
    swap(i, winnerIdx);
    switch(probabilisticChoice) {
    case UNIFORM:
      break;
    case BY_FITNESS:
      /* It re-computes the density of citizens which are not extracted yet. */
      computeDensity(i+1, populationSize);
      /* It re-computes, for those citizens, the distribution function. */ 
      computeDistributionFunction();
      break;
    }
  }
  switch(probabilisticChoice) {
  case UNIFORM:
    break;
  case BY_FITNESS:
    /* It restores the density and the distribution function. */
    computeDensity(0, populationSize);
    computeDistributionFunction();
    break;
  }
}

void StringPopulation::rankSelection(Population* _nextGeneration) { 
  int selectionSize = (int) (populationSize * (1.0f - crossoverRate));
  StringCitizen* citizen = 0;
  Citizen *temp = 0;
  int citizenIdx = 0;
  int rnd = 0;
  bool stop = false;
  /* It copy the population and sort the copied vector by fitness increasing. */
  vector<Citizen*> citizens = population;
  sortByFitness(citizens);
  /* It computes a modified distribution function. */
  vector<int> distributionF;
  /* It selects selectionSize citizens. */
  for(int i = 0; i < selectionSize; i++) {
    /* It computes distributionF for each citizen selection. 
     * Values are: 1 3 6 10 15 21 28 ... (rank(n-1)+n).
     * The best citizen (the first) has rank equals to (rank(n-1)+n), the worst one (last) has rank equal to 1. */
    distributionF = vector<int>();
    distributionF.push_back(1);
    for(int j = i + 1; j < citizens.size(); j++) {
      distributionF.push_back(j - i + 1 + distributionF[j - i - 1]);
      //cout << " dist[" << j - i << "]=" << distributionF[j - i ] << endl;
    }
    stop = false;
    /* It chooses a random value in [0, distributionF.back()). */
    switch(randomGeneratorChoice) {
    case RAN0 :      
      rnd = rand() % distributionF.back();
      break;
    case MT19937AR :
      rnd = genrand_int32() % distributionF.back();
      break;
    }
    /* It finds the index of the citizen. Note that the vectors citizens
     * and distributionF are inverted each other. */
    for(int j = 0; j < distributionF.size() && !stop; j++) {
      if(rnd < distributionF[j]) {
	citizenIdx = citizens.size() - j - 1;
	stop = true;
      }
    }
    //cout << "CIT idx = " << citizenIdx << endl;
    /* It adds the new citizen. */
    citizen = new StringCitizen();
    citizen->setString((dynamic_cast<StringCitizen*>(citizens[citizenIdx]))->getString());
    citizen->setFitness(citizens[i]->getFitness());
    _nextGeneration->addCitizen(citizen);
    /* It moves the extracted citizen in position i-th and 
     * holds the sorting. */ 
    temp = citizens[citizenIdx];
    for(int k = citizenIdx; k > i; k--) {
      citizens[k]= citizens[k-1];
    }
    citizens[i] = temp;
  }
}  



/***********************************
*********** CROSSOVER **************
************************************/ 

void StringPopulation::singlePointCrossover(Population* _nextGeneration) { 
  int crossoverSize = (int)(populationSize * crossoverRate);
  /* To prevent truncament */
  if(crossoverSize + (int) (populationSize * (1.0f - crossoverRate)) == populationSize - 1) {
    crossoverSize++;
  }
  int mask = 0, i1 = 0, i2 = 0;
  StringCitizen* offspring = 0, *parent1 = 0, *parent2 = 0;
  string temp = "";
  for(int i = 0; i < crossoverSize / 2; i++) {
    /* It chooses two parents. */
    i1 = makeProbabilisticChoice(i);
    i2 = makeProbabilisticChoice(i);
    /* It chooses the single point. */
    switch(randomGeneratorChoice) {
    case RAN0 :
      mask = rand() % target.size();
      break;
    case MT19937AR :
      mask = genrand_int32() % target.size(); 
      break;
    }
    parent1 = dynamic_cast<StringCitizen*>(population[i1]);
    parent2 = dynamic_cast<StringCitizen*>(population[i2]);
    /* It makes the crossover between parent1 and parent2. */
    /* It sets the first child */
    temp = parent1->getString().substr(0, mask) +
           parent2->getString().substr(mask);
    offspring = new StringCitizen();
    offspring->setString(temp);
    _nextGeneration->addCitizen(offspring);
    /* It sets the second child */
    temp = parent2->getString().substr(0, mask) +
      parent1->getString().substr(mask);
    offspring = new StringCitizen();
    offspring->setString(temp);
    _nextGeneration->addCitizen(offspring);
    /* It swaps the parents' indexes to avoid to choose them again. */ 
    swap(i*2, i1);
    swap(i*2+1, i2);
    switch(probabilisticChoice) {
    case UNIFORM:
      break;
    case BY_FITNESS:
      /* It re-computes the density and the distributionFunction. 
       * considering only citizens not already extracted. */
      computeDensity(i*2+2, populationSize);
      computeDistributionFunction();
      break;
    }
  }
}


void StringPopulation::twoPointCrossover(Population* _nextGeneration) { 
  int crossoverSize = (int)(populationSize * crossoverRate);
  /* To prevent truncament */
  if(crossoverSize + (int) (populationSize * (1.0f - crossoverRate)) == populationSize - 1) {
    crossoverSize++;
  }
  int mask1 = 0, mask2, i1 = 0, i2 = 0;
  StringCitizen* offspring = 0, *parent1 = 0, *parent2 = 0;
  string temp = "";
  for(int i = 0; i < crossoverSize / 2; i++) {
    /* It chooses two parents. */
    i1 = makeProbabilisticChoice(i);
    i2 = makeProbabilisticChoice(i);
    /* It chooses the two points. */
    switch(randomGeneratorChoice) {
    case RAN0 :
      mask1 = rand() % target.size();
      mask2 = rand() % target.size();
      break;
    case MT19937AR :
      mask1 = genrand_int32() % target.size(); 
      mask2 = genrand_int32() % target.size();
      break;
    }
    parent1 = dynamic_cast<StringCitizen*>(population[i1]);
    parent2 = dynamic_cast<StringCitizen*>(population[i2]);
    /* It gives and order between mask1 and mask2. On details mask1 < mask2. */
    if(mask1 > mask2) {
      int temp = mask1;
      mask1 = mask2;
      mask2 = temp;
    }
    /* It makes the crossover between parent1 and parent2. */
    /* It sets the first child */
    temp = parent1->getString().substr(0, mask1) +
      parent2->getString().substr(mask1, mask2 - mask1) +
      parent1->getString().substr(mask2);
    offspring = new StringCitizen();
    offspring->setString(temp);
    _nextGeneration->addCitizen(offspring);
    /* It sets the second child */
    temp = parent2->getString().substr(0, mask1) +
      parent1->getString().substr(mask1, mask2 - mask1) +
      parent2->getString().substr(mask2);
    offspring = new StringCitizen();
    offspring->setString(temp);
    _nextGeneration->addCitizen(offspring);
    /* It swaps the parents' indexes to avoid to choose them again. */ 
    swap(i*2, i1);
    swap(i*2+1, i2);
    switch(probabilisticChoice) {
    case UNIFORM:
      break;
    case BY_FITNESS:
      /* It re-computes the density and the distributionFunction. 
       * considering only citizens not already extracted. */
      computeDensity(i*2+2, populationSize);
      computeDistributionFunction();
      break;
    }
  }
}

void StringPopulation::uniformCrossover(Population* _nextGeneration) {
  int crossoverSize = (int)(populationSize * crossoverRate);
  /* To prevent truncament */
  if(crossoverSize + (int) (populationSize * (1.0f - crossoverRate)) == populationSize - 1) {
    crossoverSize++;
  }
  string mask = "", temp1 = "", temp2 = "";
  char tmp = '0';
  int i1 = 0, i2 = 0;
  StringCitizen* offspring = 0, *parent1 = 0, *parent2 = 0;
  for(int i = 0; i < crossoverSize / 2; i++) {
    mask.erase();
    /* It chooses two parents. */
    i1 = makeProbabilisticChoice(i);
    i2 = makeProbabilisticChoice(i);
    /* It chooses a random binary string of length equals to target.size(). */
    switch(randomGeneratorChoice) {
    case RAN0 :
      for(int j = 0; j < target.size(); j++) {
	mask.push_back(rand() % 2 + '0');
      }
      break;
    case MT19937AR :
      for(int j = 0; j < target.size(); j++) {
	mask.push_back(genrand_int32() % 2 + '0');
      }
      break;
    }
    parent1 = dynamic_cast<StringCitizen*>(population[i1]);
    parent2 = dynamic_cast<StringCitizen*>(population[i2]);
    temp1 = parent1->getString();
    temp2 = parent2->getString();
    /* It makes the crossover between parent1 and parent2. */
    for(int j = 0; j < target.size(); j++) {
      /* It crosses parents' representations on index j iff mask[j] = 1 . */
      if(mask[j] == '1') {
	tmp = temp1[j];
	temp1[j] = temp2[j];
	temp2[j] = tmp;
      }
    }
    /* It sets the first child */
    offspring = new StringCitizen();
    offspring->setString(temp1);
    _nextGeneration->addCitizen(offspring);
    /* It sets the second child */
    offspring = new StringCitizen();
    offspring->setString(temp2);
    _nextGeneration->addCitizen(offspring);
    /* It swaps the parents' indexes to avoid to choose them again. */ 
    swap(i*2, i1);
    swap(i*2+1, i2);
    switch(probabilisticChoice) {
    case UNIFORM:
      break;
    case BY_FITNESS:
      /* It re-computes the density and the distributionFunction. 
       * considering only citizens not already extracted. */
      computeDensity(i*2+2, populationSize);
      computeDistributionFunction();
      break;
    }
  }
}



/***********************************
************ MUTATION **************
************************************/ 

void StringPopulation::mutation() {
  double mutation = 0.0;
  /* It computes the percentage of mutation. */
  switch(randomGeneratorChoice) {
  case RAN0 :	   
    mutation = RAND_MAX * mutationRate;   
    break;
  case MT19937AR : 
    mutation = 0xffffffff * mutationRate; 
    break;
  }
  int position = 0;
  int delta = 0;
  for(int i = 0; i < populationSize; i++) {
    switch(randomGeneratorChoice) {
    case RAN0 :
      if(rand() < mutation) {
	/* It computes the position and the delta to mutate. */
	position = rand() % target.size();
	delta = (rand() % 94) + 32;
	/* It mutates the i-th citizen. */
	mutate(population[i], position, delta);
      }
      break;
    case MT19937AR : 
      if(genrand_int32() < mutation) {
	/* It computes the position and the delta to mutate. */
	position = genrand_int32() % target.size();
	delta = (genrand_int32() % 94) + 32;
	/* It mutates the i-th citizen. */
	mutate(population[i], position, delta);
      }
      break;
    }
  }
}

void StringPopulation::mutate(Citizen* _citizen, const int& _position, const int& _delta) {
  string temp = (dynamic_cast<StringCitizen*>(_citizen))->getString();
  temp[_position] = (char)(((temp[_position] + _delta) % 94) + 32);
  (dynamic_cast<StringCitizen*>(_citizen))->setString(temp);
}



/***********************************
 ************* FITNESS *************
 ***********************************/ 

void StringPopulation::linearFitness() {
  int fitness = 0;
  string temp = "";
  fitnessAV = 0.0;
  int bestFitnessInd = 0;
  /* It computes the average and the fitness for each citizen. */
  for(int i = 0; i < populationSize; i++) {
    fitness = 0;
    temp = (dynamic_cast<StringCitizen*>(population[i]))->getString();
    for(int j = 0; j < target.size(); j++) {
      /* The linear fitness formula. 
       * It computes the distance between chars. */
      fitness += abs(temp[j] - target[j]);
    }
    population[i]->setFitness(fitness);
    fitnessAV += fitness;
    /* It computes the best citizen. */
    if(fitness < population[bestFitnessInd]->getFitness()) {
      bestFitnessInd = i;
    }
  }
  if(populationSize > 0) {
    fitnessAV /= populationSize;
    bestCitizen = population[bestFitnessInd];
  }
}

void StringPopulation::squareFitness() {
  int fitness = 0;
  string temp = "";
  fitnessAV = 0.0;
  int bestFitnessInd = 0;
  /* It computes the squared average and the squared fitness for each citizen. */
  for(int i = 0; i < populationSize; i++) {
    fitness = 0;
    temp = (dynamic_cast<StringCitizen*>(population[i]))->getString();
    for(int j = 0; j < target.size(); j++) {
      /* The squared fitness formula. 
       * It computes the squared distance between chars. */
      fitness += (int)(pow(temp[j] - target[j], 2));
    }
    population[i]->setFitness(fitness);
    fitnessAV += fitness;
    /* It computes the best citizen. */
    if(fitness < population[bestFitnessInd]->getFitness()) {
      bestFitnessInd = i;
    }
  }
  if(populationSize > 0) {
    fitnessAV /= populationSize;
    bestCitizen = population[bestFitnessInd];
  }
}
