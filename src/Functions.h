/*
 * Functions.h
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





#ifndef FUNCTIONS_H
#define FUNCTIONS_H



#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/timeb.h>
#include "Population.h"
#include "StringPopulation.h"
#include "Citizen.h"
extern "C" {
#include "mt19937ar.h"
}
using namespace std;




/* It returns the seconds and the milliseconds between the times _t2 and _t1. */
void getRunTime(const timeb& _t1, const timeb& _t2, int& _s, int& _ms);


/**
 * It converts a numeric string into an non negative integer. It returns -1 if some
 * errors are found.
 */
int convert(const string& _number);

/** 
 * This is the genetic algorithm. It receives the configuration parameters and makes
 * generations until a threshold is reached or a solution is found.
 * It returns the last population got. 
 */
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
			     const int& _fitnessChoice);


#endif
