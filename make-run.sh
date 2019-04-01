#!/bin/bash

# $1 - how many processes to be launched
# $2 - the character to be searched
# $3 - the time between the launch the sender and the receivers

./bin/sender.out $1 $2 &
sleep $3
for (( i = 0; i < $1; i++ )); do
	./bin/receiver.out $i $1 &
done
