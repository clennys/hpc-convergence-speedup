#ifndef DAMPED_JACOBI_H_
#define DAMPED_JACOBI_H_

void damped_jacobi(double *mat, double *x, double *b, int omega, int mat_dim,
                   int my_rank, int nr_proc);
void damped_jacobi_setup(double *mat, double *x_new, double *b, double *sub_mat,
                         int mat_dim, int block_length, int my_rank);
void jacobi_step(double *sub_mat, double *x, double *b, double *x_new,
                 int block_length, int mat_dim, int omega, int my_rank);

#endif // !DAMPED_JACOBI_H_
