
# GA demo

Author: Piero Dalle Pezze

Year: 2007

Course: Intelligent Systems, Dept. of Pure and Applied Mathematics, University of Padova, Italy

[![GPLv2 License](http://img.shields.io/badge/license-GPLv2-blue.svg)](https://www.gnu.org/licenses/gpl.html)


## ABSTRACT
This package shows how a genetic algorithm works. It solves
the simple problem of finding a given string (target) in a space of random strings 
of the same length as the target one. The alphabet consists of 90 chars.

## LICENSE
geneticalgorithm is provided under the GPL v2 license. You can read a copy in the file LICENSE. 
The implementation of the Mersenne Twister algorithm is under BSD license. 
You can read a copy in the file mt19937ar.c, located inside the folder src/.

## INSTALLATION
To install geneticalgorithm, you need to run:
```
make 
```

## EXECUTION
To execute, see 
```
./geneticalgorithm --help.
```

## CONFIGURATION FILE
geneticalgorithm needs two files to run. The first one contains values to configure 
the genetic algorithm and other choices. The user can choose the algorithms to make 
the selection and crossover, to extract elements, to compute the fitness. The second 
one will contain results. A complete configuration file is made of:
```
# The fitness threshold [0..n)
fitness_threshold	  0

# The size of the population (0..n)
population_size		  2000

# The threshold of the number of iterations (0..n)
iterations_threshold	  20000

# The rate of mutation [0.0 .. 1.0)
mutation_rate		  0.25f

# The rate of crossover [0.0 .. 1.0)
crossover_rate		  0.80f

# The selection algorithm to use. You can choose one of them.
selection_choice	  ELITISM | ROULETTE_WHEEL_SELECTION | TOURNAMENT_SELECTION | RANK_SELECTION

# The crossover algorithm to use. You can choose one of them.
crossover_choice	  SINGLE_POINT_CROSSOVER | TWO_POINT_CROSSOVER | UNIFORM_CROSSOVER

# The random generator algorithm to use. You can choose one of them.
generator_choice	  RAN0 | MT19937AR

# The probabilistic variable to use. You can choose one of them.
probabilistic_choice	  UNIFORM | BY_FITNESS

# The fitness function to use. You can choose one of them.
fitness_choice		  LINEAR | SQUARE

# The target to find.
target			  Hello world (genetic algorithm)!
```
where lines that begin with '#' are comments.
Some examples of configuration files are located in the folder examples/ .
