/*
 * StringCitizen.cpp
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

 
#include "StringCitizen.h"



/************************************** 
 **** CONSTRUCTORS and DESTRUCTORS ****
 **************************************/

StringCitizen::StringCitizen(const string& _str) : Citizen(), str(_str) { }

StringCitizen::~StringCitizen() { }



/*************************************** 
 ************* GET and SET *************
 ***************************************/

string StringCitizen::getString() const { return str; }

void StringCitizen::setString(const string& _str) { str = _str; }



/*************************************** 
 ************** PRINTING ***************
 ***************************************/

void StringCitizen::print() const {
  cout << "\tCitizen: " << str << "\n\tFitness: " << getFitness(); 	
}
