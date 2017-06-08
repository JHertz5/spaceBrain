#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

namespace spaceBrain
{

void mmult_cpu(float *A,  float *B, float *C);

void madd_cpu(float *A, float *B, float *C);

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const float alpha, const float* A, const float* B, const float beta, float* C);

}

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
