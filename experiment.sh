#!/bin/bash

for i in {2..64}; do
	for j in {3..67}; do
	dim=$((j - 1))
        square=$((dim * dim))
        if [ "$square" -ge "$i" ]; then
			echo "[Processes: $i/64] :: Grid dim = $j/67 :: Matrix dim = $square"
			mpirun -np $i ./build/apps/main -p -dj -fps -gd $j >> damped_jacobi.log
			mpirun -np $i ./build/apps/main -p -gs -fps -gd $j >> gauss_seidel.log
		fi
	done
done
