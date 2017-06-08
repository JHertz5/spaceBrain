#include "maths_functions.hpp"

#include "../logger.hpp"

namespace spaceBrain
{

void mmult_cpu(float *A,  float *B, float *C)
{
/*
	for (int row = 0; row < num_input; row++)
	{
		for (int col = 0; col < num_input; col++)
		{
			float result = 0.0;
			for (int k = 0; k < num_input; k++)
			{
				result += A[row*num_input+k] * B[k*num_input+col];
			}
			C[row*num_input+col] = result;
		}
	}
 */
}

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const float alpha, const float* A, const float* B, const float beta, float* C)
{
	Logger::GetLogger()->LogMessage("\tgemm computation, transposeA = %i, transposeB = %i, alpha = %.1f, beta = %.1f", isTransposeA, isTransposeB, alpha, beta);

	//	(alpha*op(A)*op(B) + beta*C)
	//no need for size check as k is both the width of op(A) and height of op(B)
	if(!isTransposeA && !isTransposeB)
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				float result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[mIndex*k+kIndex] * B[kIndex*n+nIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else if(!isTransposeA && isTransposeB)// B only is tranpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				float result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[mIndex*k+kIndex] * B[nIndex*k+kIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else if(isTransposeA && !isTransposeB) // A only is tranpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				float result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[kIndex*m+mIndex] * B[kIndex*n+nIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else  // both transpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				float result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[kIndex*m+mIndex] * B[nIndex*k+kIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
}

}
