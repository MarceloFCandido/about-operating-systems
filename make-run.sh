#!/bin/bash

./bin/sender.out $1 $2 &
sleep 30
for (( i = 0; i < $1; i++ )); do
	./bin/receiver.out $i $1 &
done
