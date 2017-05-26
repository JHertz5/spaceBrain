#include <iostream>
#include <string>

#include "data_memory.hpp"
#include "layers/dropout_layer.hpp"
#include "layers/pooling_layer.hpp"
#include "layers/relu_layer.hpp"
#include "logger.hpp"
#include "util/filler.hpp"
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
			std::cout << "\t'fill'  \t - Blob Filling Functions" << std::endl;
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
		else if(inMessage == "fill")
		{
			std::cout << "Filler" << std::endl;
			std::cout << spaceBrain::FillerTest();
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

