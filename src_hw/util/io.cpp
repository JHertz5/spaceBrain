#include <iostream>
#include <string>

#include "../data_memory.hpp"
#include "../layers/conv_layer.hpp"
#include "../layers/dropout_layer.hpp"
#include "../layers/fully_connected_layer.hpp"
#include "../layers/pooling_layer.hpp"
#include "../layers/relu_layer.hpp"
#include "../layers/softmax_layer.hpp"
#include "../logger.hpp"
#include "../net.hpp"
#include "filler.hpp"

void printHelpMessage(bool commandLineVersion)
{
	if(commandLineVersion)
	{
		std::cout << "Usage: space_brain.exe [OPTIONS...]" << std::endl;
		std::cout << std::endl;
		std::cout << "\t -h \t Display this message" << std::endl;
		std::cout << "\t -t \t Hold testing interface open" << std::endl;
		std::cout << std::endl;
	}
	// testing interface commands
	std::cout << "Test Commands" << std::endl;
	std::cout << "\nIMPLEMENTED:" << std::endl;
	std::cout << "\t'relu' \t - ReLU Layer" << std::endl;
	std::cout << "\t'pool' \t - Pooling Layer" << std::endl;
	std::cout << "\t'conv' \t - Convolution Layer" << std::endl;
	std::cout << "\t'fc'   \t - Fully Connected Layer" << std::endl;
	std::cout << "\t'drop' \t - Dropout Layer" << std::endl;
	std::cout << "\t'data' \t - Data Memory Class" << std::endl;
	std::cout << "\t'log'  \t - Logging functionality" << std::endl;
	std::cout << "\t'fill' \t - Blob Filling Functions" << std::endl;
	std::cout << "\t'quit' \t - End Program" << std::endl;

	std::cout << "\nIn Testing:" << std::endl;
	std::cout << "\t'net' \t - Full Network Run" << std::endl;

	std::cout << "\nTODO:" << std::endl;
	std::cout << "\t'input'\t - Input Layer" << std::endl;
	std::cout << "\t'prob' \t - SoftMax Layer" << std::endl;

	std::cout << std::endl;
}

void performCommand(std::string inMessage, bool* keepRunning)
{
	const std::string notYetImplementedString =  "\t-not yet implemented";

	if(inMessage == "help")
	{
		printHelpMessage(false);
	}
	else if(inMessage == "net")
	{
		std::cout << "Network" << std::endl;
		NetTest();
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
		ConvTest();
		std::cout << std::endl;
	}
	else if(inMessage == "conv_hw")
	{
		std::cout << "Convolution Layer Hardware Test" << std::endl;
		ConvHardwareTest();
		std::cout << std::endl;
	}
	else if(inMessage == "conv_speed")
	{
		std::cout << "Convolution Speed Test" << std::endl;
		ConvSpeed();
		std::cout << std::endl;
	}
	else if(inMessage == "conv_comp")
	{
		std::cout << "Convolution Layer Implementation Comparison" << std::endl;
		ConvCompare();
		std::cout << std::endl;
	}
	else if(inMessage == "relu")
	{
		std::cout << "ReLU (Rectified Linear Unit) Layer" << std::endl;
		ReluTest();
		std::cout << std::endl;
	}
	else if(inMessage == "pool")
	{
		std::cout << "MAX Pooling Layer" << std::endl;
		PoolTest();
		std::cout << std::endl;
	}
	else if(inMessage == "fc")
	{
		std::cout << "Fully Connected/Inner Product Layer" << std::endl;
		FullyConnectedTest();
		std::cout << std::endl;
	}
	else if(inMessage == "fc_comp")
	{
		std::cout << "Fully Connected Layer Implementation Comparison" << std::endl;
		FullyConnectedCompare();
		std::cout << std::endl;
	}
	else if(inMessage == "drop")
	{
		std::cout << "Dropout Layer" << std::endl;
		DropoutTest();
		std::cout << std::endl;
	}
	else if(inMessage == "prob")
	{
		std::cout << "SoftMax Layer" << std::endl;
		SoftmaxTest();
		std::cout << std::endl;
	}
	else if(inMessage == "log")
	{
		std::cout << "Logger" << std::endl;
		LoggerTest();
		std::cout << std::endl;
	}
	else if(inMessage == "data")
	{
		std::cout << "DataMemory" << std::endl;
		DataMemoryTest();
		std::cout << std::endl;
	}
	else if(inMessage == "fill")
	{
		std::cout << "Filler" << std::endl;
		FillerTest();
		std::cout << std::endl;
	}
	else if(inMessage == "quit")
	{
		Logger::CloseLogger();
		*keepRunning = false;
	}
	else
	{
		std::cout << "COMMAND NOT RECOGNISED" << std::endl;
		std::cout << std::endl;
	}
}
