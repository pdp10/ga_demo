/*
 * Functions.cpp
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




#include "Functions.h"


void getRunTime(const timeb& _t1, const timeb& _t2, int& _s, int& _ms) { 
  _s  = (int)(_t2.time - _t1.time);
  _ms = (int)(_t2.millitm - _t1.millitm);
  if(_s < 0) _s = 60 + _s;
  if(_ms < 0) {
    _s--;
    _ms = 1000 + _ms;
  }
}


int convert(const string& _number) {
  bool error = false;
  int num = 0;
  char temp = '0';
  for(int i = _number.length() - 1; i >= 0 && !error; i--) {
    temp = _number[i];
    if(temp < '0' || temp > '9') {
      error = true;
    } else {
      num = num + (int)pow(10, (_number.length() - i - 1))*(int)(temp - '0');
    }
  }
  if(error) { return -1; }
  return num;
}


Population* geneticAlgorithm(const bool& _statisticTest,
			     ofstream& _fs,
			     const timeb& _start,
			     int& _i,
			     const int& _iterations,
			     const int& _fitnessThreshold,
			     const int& _populationSize,
			     const string& _target,
			     const float& _crossoverRate,
			     const float& _mutationRate,
			     const int& _selectionChoice,
			     const int& _crossoverChoice,
			     const int& _generatorChoice,
			     const int& _probabilisticChoice,
			     const int& _fitnessChoice) {

  int s = 0, ms = 0;
  timeb timeNow;
  /* It initializes the random generator. */
  switch(_generatorChoice) {
  case Population::MT19937AR : 
    init_genrand(unsigned(time(NULL))); 
    break;
  case Population::RAN0 :	     
    srand(unsigned(time(NULL)));        
    break;
  }
  /* It initializes the first population. */
  Population* currentPopulation = new StringPopulation(_target,
						       _crossoverRate,
						       _mutationRate);
  currentPopulation->setSelectionChoice(_selectionChoice);
  currentPopulation->setCrossoverChoice(_crossoverChoice);
  currentPopulation->setRandomGeneratorChoice(_generatorChoice);
  currentPopulation->setProbabilisticChoice(_probabilisticChoice);
  currentPopulation->setFitnessChoice(_fitnessChoice);
  currentPopulation->init(_populationSize);
  currentPopulation->computeFitness();
  //currentPopulation->print();
  //cout << endl;

  /* It makes the generations. This while-loop stops when a threshold 
   * is reached or a solution is found. */
  while(_i < _iterations && 
	currentPopulation->getBest()->getFitness() > _fitnessThreshold &&
	currentPopulation->getBest()->getFitness() < currentPopulation->getFitnessAV()) {
    /* It creates the new population and sets all choices. */
    ftime(&timeNow);
    Population* nextPopulation = new StringPopulation(_target,
						      _crossoverRate, 
						      _mutationRate);
    nextPopulation->setSelectionChoice(_selectionChoice);
    nextPopulation->setCrossoverChoice(_crossoverChoice);
    nextPopulation->setRandomGeneratorChoice(_generatorChoice);
    nextPopulation->setProbabilisticChoice(_probabilisticChoice);
    nextPopulation->setFitnessChoice(_fitnessChoice);
	  
    /* It adds citizens to the new population by selection, crossover and mutation. */
    currentPopulation->selection(nextPopulation);
    currentPopulation->crossover(nextPopulation);
    nextPopulation->mutation();
    /* The new population is ready to become the current. */
    delete currentPopulation;
    currentPopulation = nextPopulation;
    currentPopulation->computeFitness();
    getRunTime(_start, timeNow, s, ms);	  
    if(!_statisticTest) {
      cout << "\nGeneration n. " << _i  << endl; 
      /* It prints the main results for this population (best citizen, average fitness). */
      currentPopulation->getBest()->print();
      cout << "\n\tAverage Fitness: " << currentPopulation->getFitnessAV() << endl;
      cout << "\tElapsed Time: " << (double)s + ((0.001) * (double)ms) << " s " << endl;
      _fs << _i << "\t\t" << (double)s + ((0.001) * (double)ms) << "\t\t\t" 
	  << currentPopulation->getFitnessAV() << "\t\t" 
	  << currentPopulation->getBest()->getFitness() << "\t\t" << 
	(dynamic_cast<StringCitizen*>(currentPopulation->getBest()))->getString() << endl;
      _fs.flush();
    }
    _i++;
  }
  return currentPopulation;
}
