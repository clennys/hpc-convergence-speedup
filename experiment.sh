#!/bin/bash

for i in {2..13}; do
	mpirun -np $i ./build/apps/main -p -dj -fps -gd 11
	mpirun -np $i ./build/apps/main -p -gs -fps -gd 11
done

./build/apps/main -s -dj -fps -gd 11
./build/apps/main -s -gs -fps -gd 11
