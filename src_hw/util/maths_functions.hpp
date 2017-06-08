#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

namespace spaceBrain
{

#define N 32

/**
 * Design principles to achieve best performance
 *
 * 1. Declare sequential access to stream data into accelerators via a hardware FIFO
 *    interface.  Otherwise, the default RAM interface requires all data to arrive
 *    before starting HLS accelerator
 */
//#pragma SDS data access_pattern(A:SEQUENTIAL, B:SEQUENTIAL, C:SEQUENTIAL)
//void mmult (float A[N*N], float B[N*N], float C[N*N]);

//#pragma SDS data access_pattern(A:SEQUENTIAL, B:SEQUENTIAL, C:SEQUENTIAL)
//void madd(float A[N*N], float B[N*N], float C[N*N]);

void mmult_cpu(const int m, const int n, const int k, float *A,  float *B, float *C);

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const float alpha, const float* A, const float* B, const float beta, float* C);

}

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
