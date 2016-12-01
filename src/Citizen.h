/*
 * Citizen.h
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
 

#ifndef CITIZEN_H
#define CITIZEN_H


#include <iostream>
using namespace std;


/** This class represents a generic citizen of a population. */
class Citizen {
	
 private:
  /** It is a measure which tells how the citizen fits the function to approximate. */
  int fitness;

 public:
  /** It constructs a generic citizen. */
  Citizen(const int& _fitness = 0);
  /** It destructs the citizen. */
  virtual ~Citizen();
  /** It returns the fitness of the citizen. */
  virtual int getFitness() const;
  /** It sets the fitness of the citizen. */
  virtual void setFitness(const int& _fitness);
  /** It returns true if this citizen has a fitness value lesser than the_citizen one. */
  bool operator<(const Citizen& _citizen) const;
  /** It prints the citizen. */
  virtual void print() const ;
	
}; // end class Citizen

#endif
