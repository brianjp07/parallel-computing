#Makefile for Lab1.cc

CC = g++
OBJ2 = lab3.cc
FLAGS = -O3 -std=c++0x -fopenmp
all: lab3

lab3: $(OBJ2)
	$(CC) $(FLAGS) $(OBJ2) -o lab3
clean:
	rm lab3
