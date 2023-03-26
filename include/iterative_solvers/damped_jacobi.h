#ifndef DAMPED_JACOBI_H_
#define DAMPED_JACOBI_H_

#include "../inputs/param.h"

namespace damped_jacobi {
namespace parallel {
void run(double *mat, double *x, double *b, param p);
void step(double *sub_mat, double *x, double *b, double *x_new, param p);
void setup(double *mat, double *x_init, double *b, double *sub_mat, param p);
void calc_xnew(double *sub_mat, double *x, double *b, double *x_new, param p);
void calc_r(double *sub_mat, double *x, double *b, double *r_local,
            double *r_gathered, param p);
bool stopping_criterion(double *r, double *x, param p);

} // namespace parallel
namespace serial {}
} // namespace damped_jacobi
#endif // !DAMPED_JACOBI_H_
