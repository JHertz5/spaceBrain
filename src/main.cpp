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
#include <stdint.h>

//#include "sds_lib.h"
#include "maths_functions/mmultadd.hpp"
#include "maths_functions/mmultaddTest.hpp"
#include "layers/relu_layer.hpp"

int main(int argc, char* argv[]){

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

	bool keepRunning = true;
	std::string inMessage;
	const std::string notYetImplementedString =  "\t-not yet implemented";

	while(keepRunning)
	{
		std::cin >> inMessage;

		// respond to command
		if(inMessage == "-h") // HELP message
		{
			std::cout << "Neural Net Help" << std::endl;
			std::cout << "'input'\t - Input Layer" << std::endl;
			std::cout << "'conv' \t - Convolution Layer" << std::endl;
			std::cout << "'relu' \t - ReLU Layer" << std::endl;
			std::cout << "'norm' \t - LRN Layer" << std::endl;
			std::cout << "'pool' \t - Pooling Layer" << std::endl;
			std::cout << "'fc'   \t - Fully Connected Layer" << std::endl;
			std::cout << "'drop' \t - Dropout Layer" << std::endl;
			std::cout << "'prob' \t - SoftMax Layer" << std::endl;
			std::cout << "'quit' \t - End Program" << std::endl;

		}
		else if(inMessage == "input")
		{
			std::cout << "Input Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "conv")
		{
			std::cout << "Convolution Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "relu")
		{
			std::cout << "ReLU (Rectified Linear Unit) Layer" << std::endl;
			spaceBrain::ReluLayer relu1("relu1", "data", "conv1");
			std::cout << relu1.type() << " " << relu1.name << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "norm")
		{
			std::cout << "LRN (Local Response Normalisation) Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "pool")
		{
			std::cout << "MAX Pooling Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "fc")
		{
			std::cout << "Fully Connected/Inner Product Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "drop")
		{
			std::cout << "Dropout Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else if(inMessage == "prob")
		{
			std::cout << "SoftMax Layer" << std::endl;
			std::cout << notYetImplementedString << std::endl;
		}
		else
		{
			std::cout << "COMMAND NOT RECOGNISED" << std::endl;
		}
	}
}
