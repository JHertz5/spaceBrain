#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

namespace spaceBrain
{

void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* input, const int* weights, int* output
);

void conv_cpu_hw(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* inputTile, const int* weightsTile, int* outputTile
);

void conv_cpu_transB(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int outputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outChannelStart, int outChannelEnd,
		int inChannelStart, int inChannelEnd,
		const int* input, const int* weights, int* output
);

void test();

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const int alpha, const int* A, const int* B, const int beta, int* C);

}

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
