/*
 * Citizen.cpp
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


#include "Citizen.h"



/************************************** 
 **** CONSTRUCTORS and DESTRUCTORS ****
 **************************************/

Citizen::Citizen(const int& _fitness) : fitness(_fitness) { }

Citizen::~Citizen() { }



/*************************************** 
 ************* GET and SET *************
 ***************************************/

int Citizen::getFitness() const { return fitness; }

void Citizen::setFitness(const int& _fitness) { fitness = _fitness; }



/*************************************** 
 ************** OPERATORS **************
 ***************************************/

bool Citizen::operator<(const Citizen& _citizen) const { 
  return fitness < _citizen.getFitness();
}



/*************************************** 
 ************** PRINTING ***************
 ***************************************/

void Citizen::print() const { cout << "Fitness: " << fitness; }
