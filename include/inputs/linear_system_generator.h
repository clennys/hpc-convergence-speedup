#ifndef LINEAR_SYSTEM_GENERATOR_H_
#define LINEAR_SYSTEM_GENERATOR_H_

#include "param.h"

namespace discretized_grid {
void five_point_stencil(double *A, double *b, param p);
void nine_point_stencil(double *A, double *b, param p);
} // namespace discretized_grid

#endif // !LINEAR_SYSTEM_GENERATOR_H_
