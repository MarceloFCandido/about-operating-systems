#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/sync-threads-with-first-method.c
	gcc src/sync-threads-with-first-method.c -o bin/sync-threads-with-first-method.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/sync-threads-with-first-method.out
	./bin/sync-threads-with-first-method.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out