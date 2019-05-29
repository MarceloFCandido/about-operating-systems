#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/common.h src/virt-to-phys.c
	gcc src/virt-to-phys.c -o bin/virt-to-phys.out -lrt

# Rules for running the code #
#------------------------------------------------------------------------------#
run: bin/virt-to-phys.out bin/virt-to-phys.out
	./bin/virt-to-phys.out

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
