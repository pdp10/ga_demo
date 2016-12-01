/*
 * main.cc
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



#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/timeb.h>
#include "Population.h"
#include "Citizen.h"
#include "Functions.h"
using namespace std;


/**
 * This appication realizes a genetic algorithm to find a string 
 * in a space of strings of the same length. It starts with a population
 * of random strings and by selection, crossover and mutation among them,
 * it obtains a population that is more similar to the string to match.
 * This algorithm stops when a threshold is reached or the solution is found.
 */
int main(int argc, char* argv[]) {

  /* Used to compute averages. */
  vector<int> generationsAV;
  vector<double> runningTimeAV;
  vector<int> fitnessAV;
  vector<double> fitnessAVAV;
  int executions = 1;
  bool statisticalTest = false;  

  /************************************************
   ************ READING of ARGUMENTS **************
   ************************************************/

  /* It reads arguments from input */
  string filein = "", fileout = "";;
  if(argc < 2 || argc > 5) {
    cout << "Sintax error.\n"
	 << "Try geneticalgorithm --help for more information.\n";
    exit(-1);
  }
  if(string(argv[1]) == "--help" || string(argv[1]) == "-h") {
    cout << "Usage: geneticalgorithm [OPTION] SOURCE DEST\n"
	 << "Rename SOURCE with your configuration file\n"
	 << "       DEST with the output file.\n\n"
	 << "Mandatory arguments to long options are "
	 << "mandatory for short options too.\n"
	 << "  -s NUM           to run a statistic test. It runs NUM times\n"
	 << "                   the same problem and computes average results.\n"
	 << "  -h, --help       display this help and exit\n"
	 << "      --version    output version information and exit\n";
    return 0;
  }
  if(string(argv[1]) == "--version") { 
    cout << "geneticalgorithm 1.0\n"
	 << "Copyright (C) 2007 Piero Dalle Pezze <->\n"
	 << "This is free software.  You may redistribute copies of it under the terms of\n"
	 << "the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n"
	 << "There is NO WARRANTY, to the extent permitted by law.\n\n"
	 << "Written by Piero Dalle Pezze.\n";
    return 0;
  }
  if(string(argv[1]) == "-s") {
    statisticalTest = true;
    executions = convert(string(argv[2]));
    filein  = string(argv[3]);
    fileout = string(argv[4]);
    if(executions < 1) {
      cout << "\nError in -s NUM. NUM must be greater than 0\n";
      executions = 1;
    }
  } else {
    if(argc == 3) {
      filein  = string(argv[1]);
      fileout = string(argv[2]);
    } else {
	cout << "Sintax error.\n"
	     << "Try geneticalgorithm --help for more information.\n";
	exit(-1);
      }
  }


  /* Until now, all is ok */
  cout << "\n\t\t******* GENETIC ALGORITHM *******\n\n";
  cout << "\nCopyright (C) Piero Dalle Pezze 2007 <->\n\n"
       << "geneticalgorithm is free software.\n\n"
       << "You may redistribute it and/or modify it under the terms of the\n"
       << "GNU General Public License, as published by the Free Software\n"
       << "Foundation; either version 2 of the License, or (at your option)\n"
       << "any later version.\n\n"
       << "geneticalgorithm is distributed in the hope that it will be useful,\n"
       << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
       << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
       << "See the GNU General Public License for more details.\n\n"
       << "You should have received a copy of the GNU General Public License\n"
       << "along with geneticalgorithm.  If not, write to:\n"
       << "The Free Software Foundation, Inc.,\n"
       << "51 Franklin Street, Fifth Floor\n"
       << "Boston, MA  02110-1301, USA.\n\n";



  /************************************************
   *************** INITIALIZATION *****************
   ************************************************/

  /* This is the default instatiation, used to prevent user's configuration errors or missings. */
  int fitnessThreshold = 0;
  int populationSize = 1000;
  int iteration = 0;
  int iterations = 50000;
  string target = "Genetic Algorithm";
  //string target = "void StringPopulation::mutation() {double mutation = RAND_MAX * mutationRate;int position = 0;int delta = 0;string temp;for(int i = 0; i < populationSize; i++) {if(rand() < mutation) { position = rand() % target.size();delta = (rand() % 94) + 32;temp = (dynamic_cast<StringCitizen*>(population[i]))->getString(); temp[position] = (char)(((temp[position] + delta) % 94) + 32); (dynamic_cast<StringCitizen*>(population[i]))->setString(temp);}} }";
  /* To configure parameters. */
  float mutationRate = 0.20f;
  float crossoverRate = 0.80f;
  /* To configure algorithms. */
  int selectionChoice = 0;
  int crossoverChoice = 2;
  int generatorChoice = 1;
  int probabilisticChoice = 1;
  int fitnessChoice = 0;
  /* To get run time. */
  int s = 0, ms = 0;
  timeb t1, t2;

  /* It reads from filein the user's configuration.
   * Before, it reads and prints on shell the filein if it exists.
   * After, it loads that file initializing all parameters. */
  string line, choice = "";
  ifstream myfile (filein.c_str());
	
  cout << "\nINITIALIZATION"
       << "\n--------------\n";
  cout << "\nReading file " << filein << "..." << endl;
  /* It prints the file. */
  if(myfile.is_open()) {
    while(!myfile.eof()) {
      getline (myfile,line);
      cout << "\t" << line << endl;
    }
    myfile.close();
    cout << "\n.......................................[OK]\n";
  }
  else { 
    cout << "\nUnable to open file\n";
    exit(-1);
  }

  cout << "\nLoading file " << filein << "..." << endl;
  myfile.open(filein.c_str());
  if (myfile.is_open()) {
    while(!myfile.eof()) {
      myfile >> line;
      if(line.substr(0,1) == "#" || 
	 line.substr(0,1) == " " ||
	 line.substr(0,1) == "\n") {  /* This is a comment or empty line. Do nothing. */ }
      else if(line == "fitness_threshold") { myfile >> fitnessThreshold; }
      else if(line == "population_size") { myfile >> populationSize; }
      else if(line == "iterations_threshold") { myfile >> iterations; }
      else if(line == "mutation_rate") { myfile >> mutationRate; }
      else if(line == "crossover_rate") { myfile >> crossoverRate; }
      else if(line == "selection_choice") {
	myfile >> choice;
	if(choice == "ELITISM") { selectionChoice = 0; }
	else if(choice == "ROULETTE_WHEEL_SELECTION") { selectionChoice = 1; }
	else if(choice == "TOURNAMENT_SELECTION") { selectionChoice = 2; }
	else if(choice == "RANK_SELECTION") { selectionChoice = 3; }
	else { 
	  cout << "\n\tError in selection_choice."
	       << "\n\tAvailable choices are:"
	       << "\n\t\t - ELITISM"
	       << "\n\t\t - ROULETTE_WHEEL_SELECTION"
	       << "\n\t\t - TOURNAMENT_SELECTION"
	       << "\n\t\t - RANK_SELECTION"
	       << "\n\tDefault instatiation : ELITISM\n"; 
	}
      }
      else if(line == "crossover_choice") {
	myfile >> choice;
	if(choice == "SINGLE_POINT_CROSSOVER") { crossoverChoice = 0; }
	else if(choice == "TWO_POINT_CROSSOVER") { crossoverChoice = 1; }
	else if(choice == "UNIFORM_CROSSOVER") { crossoverChoice = 2; }
	else { 
	  cout << "\n\tError in crossover_choice."
	       << "\n\tAvailable choices are:"
	       << "\n\t\t - SINGLE_POINT_CROSSOVER"
	       << "\n\t\t - TWO_POINT_CROSSOVER"
	       << "\n\t\t - UNIFORM_CROSSOVER"
	       << "\n\tDefault instatiation : UNIFORM_CROSSOVER\n"; 
	}
      }
      else if(line == "generator_choice") {
	myfile >> choice;
	if(choice == "RAN0") { generatorChoice = 0; }
	else if(choice == "MT19937AR") { generatorChoice = 1; }
	else { 
	  cout << "\n\tError in generator_choice."
	       << "\n\tAvailable choices are:"
	       << "\n\t\t - RAN0"
	       << "\n\t\t - MT19937AR"
	       << "\n\tDefault instatiation : RAN0\n"; 
	}
      }
      else if(line == "probabilistic_choice") {
	myfile >> choice;
	if(choice == "UNIFORM") { probabilisticChoice = 0; }
	else if(choice == "BY_FITNESS") { probabilisticChoice = 1; }
	else { 
	  cout << "\n\tError in probabilistic_choice."
	       << "\n\tAvailable choices are:"
	       << "\n\t\t - UNIFORM"
	       << "\n\t\t - BY_FITNESS"
	       << "\n\tDefault instatiation : BY_FITNESS\n"; 
	}
      }
      else if(line == "fitness_choice") {
	myfile >> choice;
	if(choice == "LINEAR") { fitnessChoice = 0; }
	else if(choice == "SQUARE") { fitnessChoice = 1; }
	else { 
	  cout << "\n\tError in fitness_choice."
	       << "\n\tAvailable choices are:"
	       << "\n\t\t - LINEAR"
	       << "\n\t\t - SQUARE"
	       << "\n\tDefault instatiation : LINEAR\n"; 
	}
      }
      else if(line == "target") {
	getline(myfile, target);
	/* It removes tab and white spaces. */
	while(target.find('\t') == 0 || target.find(' ') == 0) {
	  target.erase(0,1);
	}
      }
    }
    cout << "\n.......................................[OK]\n";
    myfile.close();
  }
  else { 
    cout << "\nUnable to open file\n";
    exit(-1);
  }
  cout << "\nInitialization completed!\n";




  /*********************************************
   *************** GENERATIONS *****************
   *********************************************/

  /* The initialization is completed. Now the genetic algorithm is ready to start.
   * Results are also printed on file to study them after. */
  ofstream fs(fileout.c_str());
  if(statisticalTest) {
    cout << "\n\nGENERATION of STATISTIC TESTS"
	 << "\n-----------------------------\n\n";
  } else {
    cout << "\n\nGENERATION of POPULATIONS"
	 << "\n-------------------------\n";
    cout << "\nStarting generations...\n";    
    fs << "\n# This file was autogenerated.\n\n\n"
       << "Generation\tElapsed_Time (s)\tAverage_Fitness\tBest_Fitness\tBest_Citizen\n"
       << "----------\t----------------\t---------------\t------------\t------------\n";
  }
  Population* population = 0;
  for(int g = 0; g < executions; g++) {
    ftime(&t1);
    population = geneticAlgorithm(statisticalTest,
					      fs,
					      t1,
					      iteration,
					      iterations,
					      fitnessThreshold,
					      populationSize,
					      target,
					      crossoverRate,
					      mutationRate,
					      selectionChoice,
					      crossoverChoice,
					      generatorChoice,
					      probabilisticChoice,
					      fitnessChoice);
    ftime(&t2);
    getRunTime(t1, t2, s, ms);
    if(statisticalTest) {
      generationsAV.push_back(iteration);
      runningTimeAV.push_back((double)s + (0.001*(double)ms));
      fitnessAV.push_back(population->getBest()->getFitness());
      fitnessAVAV.push_back(population->getFitnessAV()); 
      cout << "Test n. " << g << " completed." << endl;
      iteration = 0;
      delete population;
      population = 0;
    }
  }
  if(statisticalTest) {
    cout << "\nAll Tests completed!\n\n";
  } else {
    cout << "\nGenerations completed!\n\n";
  }

  /**************************************************
   ************* PRINTING OF RESULTS ****************
   **************************************************/

  fs  << "\n\n\n\n  ********** CONFIGURATION SUMMARY **********\n\n"
      << "Fitness Threshold:\t\t" << fitnessThreshold << endl
      << "Population Size:\t\t" << populationSize << endl
      << "Iterations Threshold:\t\t" << iterations << endl
      << "Target:\t\t\t\t" << target << endl
      << "Target Length:\t\t\t" << target.length() << endl
      << "Mutation Rate:\t\t\t" << mutationRate << endl
      << "Crossover Rate:\t\t\t" << crossoverRate << endl;
  switch(selectionChoice) {
  case Population::ELITISM:
    fs << "Selection Choice:\t\tELITISM\n";
    break;
  case Population::ROULETTE_WHEEL_SELECTION:
    fs << "Selection Choice:\t\tROULETTE_WHEEL_SELECTION\n";
    break;
  case Population::TOURNAMENT_SELECTION:
    fs << "Selection Choice:\t\tTOURNAMENT_SELECTION\n";
    break;
  case Population::RANK_SELECTION:
    fs << "Selection Choice:\t\tRANK_SELECTION\n";
    break;
  }
  switch(crossoverChoice) {
  case Population::SINGLE_POINT_CROSSOVER:
    fs << "Crossover Choice:\t\tSINGLE_POINT_CROSSOVER\n";
    break;
  case Population::TWO_POINT_CROSSOVER:
    fs << "Crossover Choice:\t\tTWO_POINT_CROSSOVER\n";
    break;
  case Population::UNIFORM_CROSSOVER:
    fs << "Crossover Choice:\t\tUNIFORM_CROSSOVER\n";
    break;
  }
  switch(generatorChoice) {
  case Population::RAN0:
    fs << "Random Generator Choice:\tRAN0\n";
    break;
  case Population::MT19937AR:
    fs << "Random Generator Choice:\tMT19937AR (Mersenne Twister)\n";
    break;
  }
  switch(probabilisticChoice) {
  case Population::UNIFORM:
    fs << "Probabilistic Choice:\t\tUNIFORM\n";
    break;
  case Population::BY_FITNESS:
    fs << "Probabilistic Choice:\t\tBY_FITNESS\n";
    break;
  }
  switch(fitnessChoice) {
  case Population::LINEAR:
    fs << "Fitness Choice:\t\t\tLINEAR\n";
    break;
  case Population::SQUARE:
    fs << "Fitness Choice:\t\t\tSQUARE\n";
    break;
  }
  
  cout << "\n\nRESULTS"
       << "\n-------\n";
  fs  << "\n\n  ********** RESULTS **********\n\n";
  if(statisticalTest) {
    int genAV = 0, fitAV = 0;
    double rtAV = 0.0, fitAVAV = 0.0;
    for(int i = 0; i < executions; i++) {
      genAV += generationsAV[i];
      rtAV += runningTimeAV[i];
      fitAV += fitnessAV[i];
      fitAVAV += fitnessAVAV[i];
    }
    genAV /= executions;
    rtAV /= executions;
    fitAV /= executions;
    fitAVAV /= executions;
    /* The generations are completed. Below, all results are printed on both file and shell. */
    cout << "N of generations (average): \t" << genAV << endl
	 << "Running Time (average): \t" << rtAV << " s " << endl
	 << "Fitness (average): \t\t" << fitAV << endl
	 << "Average Fitness (average): \t" << fitAVAV << endl;
   
    fs << "N of generations (average): \t" << genAV << endl
       << "Running Time (average): \t" << rtAV << " s " << endl
       << "Fitness (average): \t\t" << fitAV << endl
       << "Average Fitness (average): \t" << fitAVAV << endl;
  } else {
    /* The generations are completed. Below, all results are printed on both file and shell. */
    cout << "The best citizen is:\n";
    population->getBest()->print();
    cout << endl;
    cout << "N. Generations:  " << iteration << endl;
    cout << "Average Fitness: " << population->getFitnessAV() << endl;
    cout << "GA Running Time: " << (double)s + ((0.001) * (double)ms) << " s " << endl;
   
    fs  << "Best Citizen:\t\t\t" << (dynamic_cast<StringCitizen*>(population->getBest()))->getString()
	<< "\n    Fitness:\t\t\t" << population->getBest()->getFitness() << endl
	<< "N. Generations:\t\t\t" << iteration << endl
	<< "Average Fitness:\t\t" << population->getFitnessAV() << endl
	<< "GA Running Time:\t\t" << (double)s + ((0.001) * (double)ms) << " s " << endl << endl;
  }
  fs.flush();
  fs.close();
  if(population != 0)
    delete population;
  cout << "\nAll results are saved on file " << fileout << endl << endl;
  cout << "\nExecution completed!\n\n";
  return 0;
}
