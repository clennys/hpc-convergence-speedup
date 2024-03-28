# Racing to Convergence - Comparing Parallel Speedup of Damped Jacobi and Gauss-Seidel Methods
This project was created for the numerical training course at the Faculty of Mathematics at the University of Zurich. The intent was to compare the speedup and efficiency of iterative solvers, namely damped Jacobi and Gauss-Seidel when implemented in parallel. The project was implemented in C/C++ using the MPI library. For further details, check out the corresponding paper and presentation [here](docs/).

## Results

### Parallel Damped Jacobi and Gauss-Seidel
![damped jacobi](media/damped-jacobi.png)

![Gauss-Seidel](media/gauss-seidel.png)

### Speedup
![dj_speed](media/dj_speedup-fp.png)
![gs_speed](media/gs_speedup_fp.png)

### Efficiency
![dj_eff](media/dj_efficiency-fp.png)
![gs_eff](media/gs_efficiency_fp.png)

### Timing
![40_timing](media/timing_40.png)
