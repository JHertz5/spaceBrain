/*
(c) Copyright 2013 - 2016 Xilinx, Inc. All rights reserved. 

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER 
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law: (1) THESE
MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX HEREBY
DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising
under or in connection with these materials, including for any direct, or any
indirect, special, incidental, or consequential loss or damage (including loss
of data, profits, goodwill, or any type of loss or damage suffered as a result
of any action brought by a third party) even if such damage or loss was
reasonably foreseeable or Xilinx had been advised of the possibility of the
same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES. 
 */

#include <iostream>
#include <string>

#include "data_memory.hpp"
#include "layers/dropout_layer.hpp"
#include "layers/pooling_layer.hpp"
#include "layers/relu_layer.hpp"
#include "logger.hpp"
#include "util/maths_functions_test.hpp"

int main(int argc, char* argv[])
{
	bool keepRunning = true;
	std::string inMessage;
	const std::string notYetImplementedString =  "\t-not yet implemented";

	while(keepRunning)
	{
		std::cout << "Ready for input: (input 'help' for test commands)" << std::endl;
		std::cin >> inMessage;

		// respond to command
		if(inMessage == "help") // HELP message
		{
			std::cout << "Test Commands" << std::endl;
			std::cout << "\nIMPLEMENTED:" << std::endl;
			std::cout << "\t'relu' \t - ReLU Layer" << std::endl;
			std::cout << "\t'pool' \t - Pooling Layer" << std::endl;
			std::cout << "\t'drop' \t - Dropout Layer" << std::endl;
			std::cout << "\t'maths'\t - Maths Functions" << std::endl;
			std::cout << "\t'data' \t - Data Memory Class" << std::endl;
			std::cout << "\t'log'  \t - Logging functionality" << std::endl;
			std::cout << "\t'quit' \t - End Program" << std::endl;

			std::cout << "\nTODO:" << std::endl;
			std::cout << "\t'input'\t - Input Layer" << std::endl;
			std::cout << "\t'conv' \t - Convolution Layer" << std::endl;
			std::cout << "\t'norm' \t - LRN Layer" << std::endl;
			std::cout << "\t'fc'   \t - Fully Connected Layer" << std::endl;
			std::cout << "\t'prob' \t - SoftMax Layer" << std::endl;

			std::cout << std::endl;

		}
		else if(inMessage == "input")
		{
			std::cout << "Input Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
			std::cout << std::endl;
		}
		else if(inMessage == "conv")
		{
			std::cout << "Convolution Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
			std::cout << std::endl;
		}
		else if(inMessage == "relu")
		{
			std::cout << "ReLU (Rectified Linear Unit) Layer" << std::endl;
			spaceBrain::ReluTest();
			std::cout << std::endl;
		}
		else if(inMessage == "norm")
		{
			std::cout << "LRN (Local Response Normalisation) Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
			std::cout << std::endl;
		}
		else if(inMessage == "pool")
		{
			std::cout << "MAX Pooling Layer" << std::endl;
			spaceBrain::PoolTest();
			std::cout << std::endl;
		}
		else if(inMessage == "fc")
		{
			std::cout << "Fully Connected/Inner Product Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
			std::cout << std::endl;
		}
		else if(inMessage == "drop")
		{
			std::cout << "Dropout Layer" << std::endl;
			spaceBrain::DropoutTest();
			std::cout << std::endl;
		}
		else if(inMessage == "prob")
		{
			std::cout << "SoftMax Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
			std::cout << std::endl;
		}
		else if(inMessage == "maths")
		{
			std::cout << "Maths Func" << std::endl;
			spaceBrain::gemmTest();
			std::cout << std::endl;
		}
		else if(inMessage == "log")
		{
			std::string message;
			std::cout << "Logger" << std::endl;
			std::cout << "\tinput log message:" << std::endl;

			std::cin >> message;
			spaceBrain::Logger::GetLogger()->LogMessage(message);
			std::cout << std::endl;
		}
		else if(inMessage == "data")
		{
			std::cout << "DataMemory" << std::endl;
			spaceBrain::DataMemoryTest();
			std::cout << std::endl;
		}
		else if(inMessage == "quit")
		{
			spaceBrain::Logger::CloseLogger();
			keepRunning = false;
		}
		else
		{
			std::cout << "COMMAND NOT RECOGNISED" << std::endl;
			std::cout << std::endl;
		}
	} // end of while

	spaceBrain::Logger::CloseLogger();

}


/*
	int test_passed = 0;
float *A, *B, *C, *D, *D_sw;

A = (float *)sds_alloc(N * N * sizeof(float));
B = (float *)sds_alloc(N * N * sizeof(float));
C = (float *)sds_alloc(N * N * sizeof(float));
D = (float *)sds_alloc(N * N * sizeof(float));
D_sw = (float *)malloc(N * N * sizeof(float));

if (!A || !B || !C || !D || !D_sw) {
	if (A) sds_free(A);
	if (B) sds_free(B);
	if (C) sds_free(C);
	if (D) sds_free(D);
	if (D_sw) free(D_sw);
	return 2;
}

test_passed = mmult_test(A, B, C, D, D_sw);

std::cout << "TEST " << (test_passed ? "FAILED" : "PASSED") << std::endl;

sds_free(A);
sds_free(B);
sds_free(C);
sds_free(D);
free(D_sw);
 */
//return (test_passed ? -1 : 0);

