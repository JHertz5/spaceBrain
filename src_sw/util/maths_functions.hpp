#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

namespace spaceBrain
{

void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outChannelStart, int outChannelEnd,
		int inChannelStart, int inChannelEnd,
		const float* input, const float* weights, float* output
);

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const float alpha, const float* A, const float* B, const float beta, float* C);

}

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
