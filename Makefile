#Makefile for Lab1.cc

CC = g++
OBJ2 = parks_brian_disposable.cc
FLAGS = -O3 -std=c++0x -fopenmp
all: persistent disposable

disposable: $(OBJ2)
	$(CC) $(FLAGS) $(OBJ2) -o disposable
clean:
	rm disposable persistent
