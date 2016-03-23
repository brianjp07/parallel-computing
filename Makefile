#Makefile for Lab1.cc

CC = g++
OBJ1 = parks_brian_disposable.cc
OBJ2 = parks_brian_persistent.cc
FLAGS = -O3 -std=c++0x -fopenmp
all: p d

p: $(OBJ2)
	$(CC) $(FLAGS) $(OBJ2) -o persistent

d: $(OBJ1)
	$(CC) $(FLAGS) $(OBJ1) -o disposable
clean:
	rm persistent disposable
