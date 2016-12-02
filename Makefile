CXX = g++
CC = gcc
CFLAGS = -Wall -c
CXXFLAGS = -Wall -c
LDFLAGS = -Wall -g

SRC = ./src/

CSOURCES = \
    $(SRC)*.c

CXXSOURCES = \
    $(SRC)*.cpp

OBJECTS = *.o

OUTPUT = geneticalgorithm

all: ga

ga: 
	$(CC) $(CFLAGS) $(CSOURCES)
	$(CXX) $(CXXFLAGS) $(CXXSOURCES)
	$(CXX) $(LDFLAGS) -o $(OUTPUT) $(OBJECTS)
	$(RM) ./*.o

.PHONY: clean
clean:
	$(RM) ./*.o
	$(RM) $(OUTPUT)
