#Makefile for Lab1.cc

CC = g++
OBJ = parks_brian_persistent.cc
FLAGS = -O3 -std=c++0x
all: persistent

persistent:	$(OBJ)
	$(CC) $(FLAGS) $(OBJ)

clean:
	rm a.out
