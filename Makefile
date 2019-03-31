#!/bin/bash

# Rules for compiling the code #
#------------------------------------------------------------------------------#
main: src/sender.c src/receiver.c
	gcc src/sender.c -o bin/sender.exe -lrt
	gcc src/receiver.c -o bin/receiver.exe -lrt

# Rules for running the code #
#------------------------------------------------------------------------------#
# run: bin/sender.exe
# 	./bin/sender.exe $(nprocs) $(wchar) &
# 	sleep 30
# 	for (( i = 0; i < $(nprocs); i++ )); do
# 		./bin/receiver.exe $(i) $(nprocs) &
# 	done

# Cleaning #
#------------------------------------------------------------------------------#
.PHONY: clean
clean:
	-@ rm -fv *~

.PHONY: distclean
distclean: clean
	-@ rm -fv bin/*.exe
