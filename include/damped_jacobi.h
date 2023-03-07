#ifndef DAMPED_JACOBI_H_
#define DAMPED_JACOBI_H_

#include "../include/param.h"

void damped_jacobi(double *mat, double *x, double *b, param p);
void damped_jacobi_step(double *sub_mat, double *x, double *b, double *x_new,
                        param p);
void damped_jacobi_setup(double *mat, double *x_init, double *b,
                         double *sub_mat, param p);
void damped_jacobi_xnew(double *sub_mat, double *x, double *b, double *x_new,
                        param p);
void damped_jacobi_r(double *sub_mat, double *x, double *b, double *r_local,
                     double *r_gathered, param p);
bool damped_jacobi_terminate(double *r, double *x, param p);
#endif // !DAMPED_JACOBI_H_
