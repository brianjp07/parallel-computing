#Makefile for Lab1.cc

CC = g++
OBJ = parks_brian_persistent.cc
OBJ2 = parks_brian_disposable.cc
FLAGS = -O3 -std=c++0x -lpthread
all: persistent disposable

persistent:	$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o persistent
disposable: $(OBJ)
	$(CC) $(FLAGS) $(OBJ2) -o disposable
clean:
	rm disposable persistent
