/*
 * StringCitizen.h
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
 
  
#ifndef STRING_CITIZEN_H
#define STRING_CITIZEN_H

#include <iostream>
#include <string>
#include "Citizen.h"
using namespace std;


/** This class represents a string citizen. */
class StringCitizen : public Citizen {
	
 private:
  /* It is a string that represents the genetic code of this citizen. */
  string str;

 public:
  /** It constructs a generic citizen. */
  StringCitizen(const string& _str = "");
  /** It destructs the citizen. */
  virtual ~StringCitizen();
  /** It returns the string of the citizen. */
  virtual string getString() const;
  /** It sets the string of the citizen. */
  virtual void setString(const string& _str);
  /** It prints the citizen. */
  virtual void print() const;

}; // end class StringCitizen

#endif
