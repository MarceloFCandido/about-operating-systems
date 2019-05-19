#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/creator.c src/reader.c
	gcc src/creator.c -o bin/creator.out -lrt
	gcc src/reader.c -o bin/reader.out -lrt

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/creator.out bin/reader.out
	./bin/creator.out &
	./bin/reader.out 0 &
	./bin/reader.out 1 &

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
