# COMP2521 Assignment 2

# You may modify this if you create additional .c and .h files

CC = gcc
CFLAGS = -Wall -Werror -g

.PHONY: all
all: game

game: game.c Agent.c Map.c Queue.c
	$(CC) $(CFLAGS) -o game game.c Agent.c Map.c Queue.c 

.PHONY: clean
clean:
	rm -f *.o game

