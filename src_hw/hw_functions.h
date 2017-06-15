/*
 * hw_functions.hpp
 *
 *  Created on: 13 Jun 2017
 *      Author: jjh113
 */

#ifndef SRC_HW_HW_FUNCTIONS_HPP_
#define SRC_HW_HW_FUNCTIONS_HPP_

typedef unsigned int uint;
typedef int bus_type;
typedef unsigned int uint;

#define OUT_ROW_TILE_SIZE_3X3 14
#define OUT_COL_TILE_SIZE_3X3 14
#define OUT_DEPTH_TILE_SIZE_3X3 4
#define IN_ROW_TILE_SIZE_3X3 OUT_ROW_TILE_SIZE_3X3+2
#define IN_COL_TILE_SIZE_3X3 OUT_COL_TILE_SIZE_3X3+2
#define IN_DEPTH_TILE_SIZE_3X3 4
#define KERNEL_SIZE_3X3 3
#define PAD_3X3 1
#define STRIDE_3X3 1

#define IO_DIMENSIONS 3         // input/output data dimensionality
#define WEIGHT_DIMENSIONS 4     // data dimensionality
#define BURST_LENGTH 14			// burst length (in data points)

#define INPUT_DATA_LENGTH 		IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3
#define WEIGHTS_DATA_LENGTH 	OUT_DEPTH_TILE_SIZE_3X3*IN_DEPTH_TILE_SIZE_3X3*KERNEL_SIZE_3X3*KERNEL_SIZE_3X3
#define OUTPUT_DATA_LENGTH		OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3

#define INPUT_START_ADDRESS 	30 // TODO change to real
#define WEIGHTS_START_ADDRESS 	0
#define OUTPUT_START_ADDRESS 	60 // TODO change to real



//#pragma SDS data copy(inputTile[ 0 : IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ], weightsTile[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ])
//#pragma SDS data zero_copy(outputTile[ 0 : OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ])
//void conv_hw_conv3x3(int inputSize, int outputSize, int inputDepth,
//		int outRowStart, int outRowEnd,
//		int outColStart, int outColEnd,
//		int outDepthStart, int outDepthEnd,
//		int inDepthStart, int inDepthEnd,
//		int inputTile [ IN_DEPTH_TILE_SIZE_3X3 * IN_ROW_TILE_SIZE_3X3 * IN_COL_TILE_SIZE_3X3 ],
//		int weightsTile [ OUT_DEPTH_TILE_SIZE_3X3 * IN_DEPTH_TILE_SIZE_3X3 * KERNEL_SIZE_3X3 * KERNEL_SIZE_3X3 ],
//		int outputTile [ OUT_DEPTH_TILE_SIZE_3X3 * OUT_ROW_TILE_SIZE_3X3 * OUT_COL_TILE_SIZE_3X3 ]
//);

void conv_hw_conv3x3( uint block_address,
		volatile bus_type *master_portA,
		//volatile bus_type *master_portB,
		uint input_addr,
		uint weights_addr,
		uint output_addr,
		uint inputSize, uint outputSize, uint inputDepth,
		uint outRowStart, uint outRowEnd, 		// outRow limits
		uint outColStart, uint outColEnd, 		// outCol limits
		uint outDepthStart, uint outDepthEnd, 	// outDepth limits
		uint inDepthStart, uint inDepthEnd, 	// inDepth limits
		uint *debug
);


#endif /* SRC_HW_HW_FUNCTIONS_HPP_ */
