#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
owner-or-wait: src/ownership-and-waiting.c
	gcc src/ownership-and-waiting.c -o bin/ownership-and-waiting.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run-first: bin/ownership-and-waiting.out
	./bin/ownership-and-waiting.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out