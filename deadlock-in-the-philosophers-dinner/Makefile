#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
owner-or-wait: src/ownership-and-waiting.c
	gcc src/ownership-and-waiting.c -o bin/ownership-and-waiting.out -lpthread

queue: src/philosophers-queue.c
	gcc src/philosophers-queue.c -o bin/philosophers-queue.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run-first: bin/ownership-and-waiting.out
	./bin/ownership-and-waiting.out

run-second: bin/philosophers-queue.out
	./bin/philosophers-queue.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out