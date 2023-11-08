FLAGS = -std=c99 -Wall -Wextra -Wpedantic
CC = g++
prog: main.o
	$(CC) $(FLAGS) main.o -o prog
main.o: main.c
	$(CC) -c $(FLAGS) -g main.c
