# COMP2521 Assignment 1 Makefile

# !!! DO NOT MODIFY THIS FILE !!!

CC = gcc
CFLAGS = -Wall -Werror -g

.PHONY: all
all: testSet

testSet: testSet.c Set.c
	$(CC) $(CFLAGS) -o testSet testSet.c Set.c

.PHONY: clean
clean:
	rm -f *.o testSet