#ifndef GAUSS_SEIDEL_H_
#define GAUSS_SEIDEL_H_

#include "../include/param.h"

void gauss_seidel_setup(double *mat, double *sub_mat, double *x_init,
                        double *x_new, double *b, param p);
void gauss_seidel_r(double *sub_mat, double *x, double *b, double *r_local,
                    param p);

void gauss_seidel_forward_substitution(double *mat_GS, double *r, double *y,
                                       int mat_dim);

void gauss_seidel_step(double *mat, double *sub_mat, double *x, double *b,
                       double *r_local, double *r_gathered, double *y_scatter,
                       double *y_local, double *x_new, param p);
void gauss_seidel(double *mat, double *x, double *b, param p);

bool gauss_seidel_terminate(double *r, double *x, param p);
#endif // !GAUSS_SEIDEL_H_
