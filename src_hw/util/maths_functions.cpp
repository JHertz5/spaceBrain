#include "maths_functions.hpp"

#include <iostream>

#include "../logger.hpp"

void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDethEnd,
		int inDepthStart, int inDepthEnd,
		const float* input, const float* weights, float* output
)
{
	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDethEnd; outDepthIndex++)
			{
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * outRowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * outColIndex + kernelCol - pad;

							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								output[(outDepthIndex * outRowEnd + outRowIndex) * outColEnd + outColIndex] += 0;
							}
							else
							{
								output[(outDepthIndex * outputSize + outRowIndex) * outputSize + outColIndex] +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weights[((outDepthIndex * inputDepth + inDepthIndex) * kernelSize + kernelRow) * kernelSize + kernelCol] *
//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
									input[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inDepthIndex][paddedRow][paddedCol];
							}
						}
					}
				}
			}
		}
	}
}

void conv_cpu_transB(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int outputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const float* input, const float* weights, float* output
)
{
	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * outRowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * outColIndex + kernelCol - pad;

							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								output[(outDepthIndex * outRowEnd + outRowIndex) * outColEnd + outColIndex] += 0;
							}
							else
							{
								output[(outDepthIndex * outputSize + outRowIndex) * outputSize + outColIndex] +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weights[((inDepthIndex * kernelSize + kernelRow) * kernelSize + kernelCol) * outputDepth + outDepthIndex] *
//									weights[inDepthIndex][kernelRow][kernelCol][outDepthIndex] *
									input[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inDepthIndex][paddedRow][paddedCol];
							}
						}
					}
				}
			}
		}
	}
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
