#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/sender.c src/receiver.c
	gcc src/sender.c -o bin/sender.out -lrt
	gcc src/receiver.c -o bin/receiver.out -lrt

# Rules for running the code #
#------------------------------------------------------------------------------#
# run: bin/sender.out
# 	./bin/sender.out $(nprocs) $(wchar) &
# 	sleep 30
# 	for (( i = 0; i < $(nprocs); i++ )); do
# 		./bin/receiver.out $(i) $(nprocs) &
# 	done

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.out
