#!/bin/bash

./bin/sender.exe $1 $2 &
sleep 30
for (( i = 0; i < $1; i++ )); do
	./bin/receiver.exe $i $1 &
done
