#!/bin/bash

for i in {2..13}; do
	for j in {5..40}; do
		echo "Processes: $i with grid dim $j"
		mpirun -np $i ./build/apps/main -p -dj -fps -gd $j >> damped_jacobi.log
		mpirun -np $i ./build/apps/main -p -gs -fps -gd $j >> gauss_seidel.log
	done
done

# ./build/apps/main -s -dj -fps -gd 11
# ./build/apps/main -s -gs -fps -gd 11
