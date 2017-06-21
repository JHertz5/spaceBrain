#define KERNEL_SIZE_3X3 3
#define PAD_3X3 1
#define STRIDE_3X3 1

#define WEIGHTS_DATA_LENGTH 9
#define INPUT_DATA_LENGTH 256
#define OUTPUT_DATA_LENGTH 196

void load_inputs_buffer(uint offset,uint address, volatile bus_type *bus, uint *buffer)
{
	bus_type int_buffer[INPUT_DATA_LENGTH];

	memcpy(int_buffer, (const bus_type *)(bus + (offset + address)/sizeof(bus_type)), INPUT_DATA_LENGTH*sizeof(bus_type));

	for (uint i=0; i<INPUT_DATA_LENGTH; i++) {
#pragma HLS loop_flatten
#pragma HLS pipeline II=1
		//#pragma HLS unroll
		buffer[i] = int_buffer[i];
	}
}

void load_weights_buffer(uint offset,uint address, volatile bus_type *bus, uint *buffer)
{
	bus_type int_buffer[WEIGHTS_DATA_LENGTH];

	memcpy(int_buffer, (const bus_type *)(bus + (offset + address)/sizeof(bus_type)), WEIGHTS_DATA_LENGTH*sizeof(bus_type));

	for (uint i=0; i<WEIGHTS_DATA_LENGTH; i++) {
#pragma HLS loop_flatten
#pragma HLS pipeline II=1
		//#pragma HLS unroll
		buffer[i] = int_buffer[i];
	}
}

//store_output_buffer(output_addr, output_buffer, kernel_info_block_address, master_portA);
void store_output_buffer(uint offset, uint *buffer, uint address, volatile bus_type *bus)
{
	bus_type int_buffer[OUTPUT_DATA_LENGTH];


	for (uint i=0; i<OUTPUT_DATA_LENGTH; i++) {
#pragma HLS pipeline II=1
		int_buffer[i] = (bus_type)buffer[i];
	}

	memcpy((bus_type *)(bus + (offset + address)/sizeof(bus_type) ), int_buffer, OUTPUT_DATA_LENGTH*sizeof(bus_type));
}



// top-level function of the design
void lloyds_kernel_top(
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
#pragma HLS TOP

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

	//	#pragma HLS INTERFACE ap_none register port=debug
	//	#pragma HLS RESOURCE core=AXI4LiteS variable=debug metadata="-bus_bundle CONFIG_BUS"

#pragma HLS RESOURCE variable=return core=AXI4LiteS	metadata="-bus_bundle CONFIG_BUS"

	uint input_buffer[INPUT_DATA_LENGTH];
	uint weights_buffer[WEIGHTS_DATA_LENGTH];
	uint output_buffer[OUTPUT_DATA_LENGTH];

	load_inputs_buffer(input_addr, block_address, master_portA, input_buffer);
	load_weights_buffer(weights_addr, block_address, master_portA, weights_buffer);

	int outRowTileSize = outRowEnd - outRowStart;
	int outColTileSize = outColEnd - outColStart;
	int outDepthTileSize = outDepthEnd - outDepthStart;
	int inDepthTileSize = inDepthEnd - inDepthStart;

	int inRowTileSize = outRowTileSize + 2;
	int inColTileSize = outColTileSize + 2;

	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
//				int result = 0;
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < KERNEL_SIZE_3X3; kernelRow++)
					{
						paddedRow = STRIDE_3X3 * outRowIndex + kernelRow - PAD_3X3;

						for(int kernelCol = 0; kernelCol < KERNEL_SIZE_3X3; kernelCol++)
						{
							paddedCol = STRIDE_3X3 * outColIndex + kernelCol - PAD_3X3;

							if(!(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize))
							{
								//outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] +=

								output_buffer[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] +=
										//								output[outDepthIndex][rowIndex][colIndex] +=
										weights_buffer[((outDepthIndex * inDepthTileSize + inDepthIndex) * KERNEL_SIZE_3X3 + kernelRow) * KERNEL_SIZE_3X3 + kernelCol] *
										//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
										input_buffer[(inDepthIndex * inRowTileSize + paddedRow) * inColTileSize + paddedCol];
								//									input[inDepthIndex][inputRow][inputCol];

							}
						}
					}
				}
			}
		}
	}

	store_output_buffer(output_addr, output_buffer, block_address, master_portA);
}
