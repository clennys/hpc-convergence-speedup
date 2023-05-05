#!/bin/bash

parallel() {
	for i in {2..64}; do
		for j in {3..80}; do
			dim=$((j - 1))
			square=$((dim * dim))
			if [ "$square" -ge "$i" ]; then
				echo "[Processes: $i/64] :: Grid dim = $j/80 :: Matrix dim = $square"
				echo "======== [Processes: $i/64] :: Grid dim = $j/80 :: Matrix dim = $square =========" >> output/"damped_jacobi.$i.log"
				mpirun -np $i ./build/apps/main -p -dj -fps -gd $j >> output/"damped_jacobi.$i.log"
				echo "======== [Processes: $i/64] :: Grid dim = $j/80 :: Matrix dim = $square =========" >> output/"gauss_seidel.$i.log"
				mpirun -np $i ./build/apps/main -p -gs -fps -gd $j >> output/"gauss_seidel.$i.log"
			fi
		done
	done
}

serial(){
		for j in {3..40}; do
			dim=$((j - 1))
			square=$((dim * dim))
			echo "[Processes: 1/1] :: Grid dim = $j/40 :: Matrix dim = $square"
			echo "======== [Processes: $i/64] :: Grid dim = $j/40 :: Matrix dim = $square =========" >> output/"damped_jacobi.$i.log"
			./build/apps/main -s -dj -fps -gd $j >> output/"damped_jacobi.1.log"
			echo "======== [Processes: $i/64] :: Grid dim = $j/40 :: Matrix dim = $square =========" >> output/"gauss_seidel.$i.log"
			./build/apps/main -s -gs -fps -gd $j >> output/"gauss_seidel.1.log"
		done

}

case $1 in
	"-p") parallel
	;;
	"-s") serial
	;;
	*) echo "select -p for parallel or -s for serial"
	;;
esac

