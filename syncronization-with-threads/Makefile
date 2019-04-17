#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main-first: src/sync-threads-with-first-method.c
	gcc src/sync-threads-with-first-method.c -o bin/sync-threads-with-first-method.out -lpthread

main-third: src/sync-threads-with-third-method.c
	gcc src/sync-threads-with-third-method.c -o bin/sync-threads-with-third-method.out -lpthread

# Rules for running the code #
#------------------------------------------------------------------------------#
run-first: bin/sync-threads-with-first-method.out
	./bin/sync-threads-with-first-method.out

run-third: bin/sync-threads-with-third-method.out
	./bin/sync-threads-with-third-method.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
