/*
 * hw_functions.hpp
 *
 *  Created on: 13 Jun 2017
 *      Author: jjh113
 */

#ifndef SRC_HW_HW_FUNCTIONS_HPP_
#define SRC_HW_HW_FUNCTIONS_HPP_

#define OUT_ROW_TILE_SIZE_3X3 14
#define OUT_COL_TILE_SIZE_3X3 14
#define OUT_DEPTH_TILE_SIZE_3X3 4
#define IN_ROW_TILE_SIZE_3X3 OUT_ROW_TILE_SIZE_3X3+2
#define IN_COL_TILE_SIZE_3X3 OUT_COL_TILE_SIZE_3X3+2
#define IN_DEPTH_TILE_SIZE_3X3 4
#define KERNEL_SIZE_3X3 3
#define PAD_3X3 1
#define STRIDE_3X3 1

//TODO give tile sizes as inputs to reduce data input
#pragma SDS data copy(inputTile[ 0 : IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ], weightsTile[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ])
#pragma SDS data zero_copy(outputTile[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ])
//#pragma SDS data access_pattern(A:SEQUENTIAL, B:SEQUENTIAL, C:SEQUENTIAL)
void conv_hw_conv3x3(int inputSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		int inputTile [ IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ],
		int weightsTile [ OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ],
		int outputTile [ OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ]
);


#endif /* SRC_HW_HW_FUNCTIONS_HPP_ */
