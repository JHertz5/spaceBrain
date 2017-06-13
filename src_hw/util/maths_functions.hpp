#ifndef SRC_UTIL_MATHS_FUNCTIONS_HPP_
#define SRC_UTIL_MATHS_FUNCTIONS_HPP_

void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* input, const int* weights, int* output
);

//#pragma SDS data copy(inputTile[ 0 : IN_DEPTH_TILE_SIZE_3X3 * inRowTileSize * inColTileSize ], weigthsTile[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ])
//#pragma SDS data zero_copy(output[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ])
//#pragma SDS data access_pattern(A:SEQUENTIAL, B:SEQUENTIAL, C:SEQUENTIAL)
//void conv_hw_conv3x3(int inputSize//, int outputSize, int inputDepth,
//		int outRowStart, int outRowEnd,
//		int outColStart, int outColEnd,
//		int outDepthStart, int outDepthEnd,
//		int inDepthStart, int inDepthEnd,
//		int inputTile [ IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ],
//		int weightsTile [ OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ],
//		int outputTile [ OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ]
//);

void conv_cpu_transB(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int outputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* input, const int* weights, int* output
);

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const int alpha, const int* A, const int* B, const int beta, int* C);

#endif /* SRC_UTIL_MATHS_FUNCTIONS_HPP_ */
