#include "maths_functions.hpp"

#include "../logger.hpp"

namespace spaceBrain
{


void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outChannelStart, int outChannelEnd,
		int inChannelStart, int inChannelEnd,
		const float* input, const float* weights, float* output
)
{
	int paddedRow, paddedCol;
	for(int rowIndex = outRowStart; rowIndex < outRowEnd; rowIndex++)
	{
		for(int colIndex = outColStart; colIndex < outColEnd; colIndex++)
		{
			for(int outChannelIndex = outChannelStart; outChannelIndex < outChannelEnd; outChannelIndex++)
			{
				for(int inChannelIndex = inChannelStart; inChannelIndex < inChannelEnd; inChannelIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * rowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * colIndex + kernelCol - pad;

							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								output[(outChannelIndex * outRowEnd + rowIndex) * outColEnd + colIndex] += 0;
							}
							else
							{
								output[(outChannelIndex * outputSize + rowIndex) * outputSize + colIndex] +=
//								output[outChannelIndex][rowIndex][colIndex] +=
									weights[((outChannelIndex * outputSize + inChannelIndex) * kernelSize + kernelRow) * kernelSize + kernelCol] *
//									weights[outChannelIndex][inChannelIndex][kernelRow][kernelCol] *
									input[(inChannelIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inChannelIndex][paddedRow][paddedCol];
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

}
