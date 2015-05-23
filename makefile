CC=clang
CFLAGS=-std=c99 -Wall

all: ga

ga: genetic_algorithm.c
	$(CC) $(CFLAGS) $^ -o $@
