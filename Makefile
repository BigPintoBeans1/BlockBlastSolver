###
### Makefile for Block Blast Solver Project
###
### Author:  Emmett Roberts & Max Antonini

## flags
CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

## blockBlastSolver executable
blockBlastSolver: blockBlast.o main.o
	${CXX} ${LDFLAGS} -o $@ $^

## blockBlast.o
blockBlast.o: blockBlast.cpp blockBlast.h
	$(CXX) $(CXXFLAGS) -c blockBlast.cpp

## main.o
main.o: main.cpp blockBlast.h
	$(CXX) $(CXXFLAGS) -c main.cpp
