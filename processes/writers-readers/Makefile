#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/check.h src/writers-readers.c 
	gcc src/writers-readers.c -o bin/writers-readers.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/writers-readers.out
	./bin/writers-readers.out $(mst) $(nw) $(nr)

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
