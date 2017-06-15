#include "../../../../../../../../space_brain/src_hw/hw_functions.h"



void conv_hw_conv3x3(
		uint block_address,
		volatile bus_type *master_portA,
		//volatile bus_type *master_portB,
		uint input_addr,
		uint weights_addr,
		uint output_addr,
		uint inputSize, uint outputSize, uint inputDepth,
		uint outRowStart, uint outRowEnd, 		// outRow limits
		uint outColStart, uint outColEnd, 		// outCol limits
		uint outDepthStart, uint outDepthEnd, 	// outDepth limits
		uint inDepthStart, uint inDepthEnd 	// inDepth limits
)
{
	// set up the axi bus interfaces
	#pragma HLS INTERFACE ap_bus port=master_portA depth=2147483648
	#pragma HLS resource variable=master_portA core=AXI4M

	//#pragma HLS INTERFACE ap_bus port=master_portB depth=0x002000
	//#pragma HLS resource core=AXI4M variable=master_portB

	#pragma HLS INTERFACE ap_none register port=input_addr
	#pragma HLS RESOURCE core=AXI4LiteS variable=input_addr metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=weights_addr
	#pragma HLS RESOURCE core=AXI4LiteS variable=weights_addr metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=output_addr
	#pragma HLS RESOURCE core=AXI4LiteS variable=output_addr metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=inputSize
	#pragma HLS RESOURCE core=AXI4LiteS variable=inputSize metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outputSize
	#pragma HLS RESOURCE core=AXI4LiteS variable=outputSize metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=inputDepth
	#pragma HLS RESOURCE core=AXI4LiteS variable=inputDepth metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outRowStart
	#pragma HLS RESOURCE core=AXI4LiteS variable=outRowStart metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outRowEnd
	#pragma HLS RESOURCE core=AXI4LiteS variable=outRowEnd metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outColStart
	#pragma HLS RESOURCE core=AXI4LiteS variable=outColStart metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outColEnd
	#pragma HLS RESOURCE core=AXI4LiteS variable=outColEnd metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outDepthStart
	#pragma HLS RESOURCE core=AXI4LiteS variable=outDepthStart metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=outDepthEnd
	#pragma HLS RESOURCE core=AXI4LiteS variable=outDepthEnd metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=inDepthStart
	#pragma HLS RESOURCE core=AXI4LiteS variable=inDepthStart metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=inDepthEnd
	#pragma HLS RESOURCE core=AXI4LiteS variable=inDepthEnd metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=block_address
	#pragma HLS RESOURCE core=AXI4LiteS variable=block_address metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS INTERFACE ap_none register port=debug
	#pragma HLS RESOURCE core=AXI4LiteS variable=debug metadata="-bus_bundle CONFIG_BUS"

	#pragma HLS RESOURCE variable=return core=AXI4LiteS	metadata="-bus_bundle CONFIG_BUS"

	//load weights
	bus_type weightsTile[WEIGHTS_DATA_LENGTH];
	for(int weightsIndex = 0; weightsIndex < WEIGHTS_DATA_LENGTH; weightsIndex++){
		weightsTile[weightsIndex] = master_portA[weightsIndex];
	}

	bus_type outputTile[OUTPUT_DATA_LENGTH];

	// compute tile convolution
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
#pragma HLS UNROLL
						for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
						{
#pragma HLS UNROLL
							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
//								outputTile[(outDepthIndex * outRowEnd + outRowIndex) * outColEnd + outColIndex] += 0;
							}
							else
							{
								// not padded
								int result =
//								output[outDepthIndex][rowIndex][colIndex] +=
									weightsTile[((outDepthIndex * inputDepth + inDepthIndex) * KERNEL_SIZE_3X3 + kernelRow) * KERNEL_SIZE_3X3 + kernelCol] *
//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
									// input taken straight from port due to large size
									master_portA[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol + INPUT_START_ADDRESS/sizeof(bus_type)];
//									input[inDepthIndex][paddedRow][paddedCol];

								// output taken straight from port due to large size
								master_portA[(outDepthIndex * OUT_ROW_TILE_SIZE_3X3 + outRowIndex) * OUT_COL_TILE_SIZE_3X3 + outColIndex + OUTPUT_START_ADDRESS/sizeof(bus_type)]
											 += result;

							}
						}
					}
				}
			}
		}
	}

}




void cnn_top(
		uint block_address,
		volatile bus_type *master_portA,
		//volatile bus_type *master_portB,
		uint input_addr,
		uint weights_addr,
		uint output_addr,
		uint inputSize, uint outputSize, uint inputDepth,
		uint outRowStart, uint outRowEnd, 		// outRow limits
		uint outColStart, uint outColEnd, 		// outCol limits
		uint outDepthStart, uint outDepthEnd, 	// outDepth limits
		uint inDepthStart, uint inDepthEnd	 	// inDepth limits
		)
{

	conv_hw_conv3x3(block_address,
					master_portA,
					input_addr,
					weights_addr,
					output_addr,
					inputSize, outputSize, inputDepth,
					outRowStart, outRowEnd, 		// outRow limits
					outColStart, outColEnd, 		// outCol limits
					outDepthStart, outDepthEnd, 	// outDepth limits
					inDepthStart, inDepthEnd);

}
