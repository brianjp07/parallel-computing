#Makefile for Lab1.cc

CC = g++
OBJ = lab1.cc
FLAGS = -O3 -std=c++0x
all: lab1

lab1:	$(OBJ)
	$(CC) $(FLAGS) $(OBJ)

clean:
	rm a.out
