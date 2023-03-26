#ifndef LINEAR_SYSTEM_GENERATOR_H_
#define LINEAR_SYSTEM_GENERATOR_H_

#include "param.h"

namespace discretized_grid {
void five_point_stencil(double *A, double *b, param p);
}

#endif // !LINEAR_SYSTEM_GENERATOR_H_
