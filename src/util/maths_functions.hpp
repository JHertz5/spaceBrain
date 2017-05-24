#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

#define N 32

enum Transpose
{
	NO_TRANSPOSE,
	TRANSPOSE
};

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

void mmult_cpu(float *A,  float *B, float *C);

void madd_cpu(float *A, float *B, float *C);

void gemm_cpu(const Transpose TransA, const Transpose TransB, const int m, const int n, const int k, const float alpha, const float* A, const float* B, const float beta, float* C);

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
