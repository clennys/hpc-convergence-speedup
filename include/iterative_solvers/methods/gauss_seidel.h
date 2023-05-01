#ifndef GAUSS_SEIDEL_H_
#define GAUSS_SEIDEL_H_

#include "../../inputs/param.h"

namespace gauss_seidel {
namespace parallel {
void setup(double *mat, double *sub_mat, double *x_init, double *x_new,
           double *b, param p);
void calc_r(double *sub_mat, double *x, double *b, double *r_local, param p);

void forward_substitution(double *mat_GS, double *r, double *y, param p);

void step(double *mat, double *sub_mat, double *x, double *b, double *r_local,
          double *r_gathered, double *y_scatter, double *y_local, double *x_new,
          param p);
int run(double *mat, double *x, double *b, param p);

bool stopping_criterion(double *r, double *x, param p);

} // namespace parallel
namespace serial {
int run(double *matrix, double *x, double *b, param p);
}

} // namespace gauss_seidel

#endif // !GAUSS_SEIDEL_H_
