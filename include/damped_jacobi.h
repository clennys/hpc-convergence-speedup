#ifndef DAMPED_JACOBI_H_
#define DAMPED_JACOBI_H_

int damped_jacobi(double *mat, double *x, double *b, double *omega, int mat_dim,
                  int my_rank, int nr_proc);
void damped_jacobi_step(double *sub_mat, double *x, double *b, double *x_new,
                        double *omega, int block_length, int mat_dim,
                        int my_rank);
void damped_jacobi_setup(double *mat, double *x_init, double *b,
                         double *sub_mat, double *omega, int mat_dim,
                         int block_length, int my_rank);
void damped_jacobi_xnew(double *sub_mat, double *x, double *b, double *x_new,
                        double *omega, int block_length, int mat_dim,
                        int my_rank);

#endif // !DAMPED_JACOBI_H_
