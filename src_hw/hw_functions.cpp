#include "hw_functions.hpp"

void conv_hw_conv3x3(int inputSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		float inputTile [ IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ],
		float weightsTile [ OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ],
		float outputTile [ OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ]
)
{

	int paddedRow, paddedCol;

	for(int kernelRow = 0; kernelRow < KERNEL_SIZE_3X3; kernelRow++)
	{
		for(int kernelCol = 0; kernelCol < KERNEL_SIZE_3X3; kernelCol++)
		{
			for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
			{
				paddedRow = STRIDE_3X3 * outRowIndex + kernelRow - PAD_3X3;

				for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
				{
					paddedCol = STRIDE_3X3 * outColIndex + kernelCol - PAD_3X3;
#pragma HLS PIPELINE
					for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
					{
						float result = 0;
#pragma HLS UNROLL
						for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
						{
#pragma HLS UNROLL
							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								outputTile[(outDepthIndex * outRowEnd + outRowIndex) * outColEnd + outColIndex] += 0;
							}
							else
							{
//								outputTile[(outDepthIndex * outputSize + outRowIndex) * outputSize + outColIndex] +=
								result +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weightsTile[((outDepthIndex * inputDepth + inDepthIndex) * KERNEL_SIZE_3X3 + kernelRow) * KERNEL_SIZE_3X3 + kernelCol] *
//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
									inputTile[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inDepthIndex][paddedRow][paddedCol];



//								if(weightsTile[((outDepthIndex * inputDepth + inDepthIndex) * KERNEL_SIZE_3X3 + kernelRow) * KERNEL_SIZE_3X3 + kernelCol] != 1 ||
//										inputTile[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol] != 1)
//								{
//								}
							}
						}
						outputTile[(outDepthIndex * outputSize + outRowIndex) * outputSize + outColIndex] = result;
					}
				}
			}
		}
	}
}
