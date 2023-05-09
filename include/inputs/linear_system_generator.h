#ifndef LINEAR_SYSTEM_GENERATOR_H_
#define LINEAR_SYSTEM_GENERATOR_H_

#include "param.h"

namespace discretized_grid {
void laplace(double *A, double *b, param p);
void L2_projection(double *A, double *b, param p);
} // namespace discretized_grid

#endif // !LINEAR_SYSTEM_GENERATOR_H_
