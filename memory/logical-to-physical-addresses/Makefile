#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/logical-to-physical-addresses.c 
	gcc src/logical-to-physical-addresses.c -o \
		bin/logical-to-physical-addresses.out -lm

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/logical-to-physical-addresses.out
	./bin/logical-to-physical-addresses.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
