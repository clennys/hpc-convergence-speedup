#!/bin/bash
current_time=$(date "+%Y.%m.%d-%H.%M.%S")

for i in {2..64}; do
	for j in {3..67}; do
		dim=$((j - 1))
        square=$((dim * dim))
        if [ "$square" -ge "$i" ]; then
			echo "[Processes: $i/64] :: Grid dim = $j/67 :: Matrix dim = $square"
			mpirun -np $i ./build/apps/main -p -dj -fps -gd $j > output/"damped_jacobi_$i.$square.$current_time.log"
			mpirun -np $i ./build/apps/main -p -gs -fps -gd $j > output/"gauss_seidel$i.$square.$current_time.log"
		fi
	done
done
