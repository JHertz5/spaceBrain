/*
 * relu_layer.cpp
 *
 *  Created on: 11 Apr 2017
 *      Author: jjh113
 */

#include "relu_layer.hpp"
#include "../logger.hpp"
#include "stdlib.h"
#include <cmath>
#include <iostream>

namespace spaceBrain
{

ReluLayer::ReluLayer(std::string name, std::string bottom, std::string top)
{
	ReluLayer::name_ = name;
	ReluLayer::bottom_ = bottom;
	ReluLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tReLU layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void ReluLayer::Forward(const Blob *bottom, const Blob *top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	bool sizeCheckPassed = true;
	for(int dimensionIndex = 0; dimensionIndex < BLOB_SHAPE_DIMENSIONS; dimensionIndex++)
	{
		sizeCheckPassed &= bottom->shape()[dimensionIndex] == top->shape()[dimensionIndex];
	}
	if(!sizeCheckPassed)
	{

		Logger::GetLogger()->LogError("ReluLayer::Forward()", "Top and bottom blob shapes do not match");
		std::cerr << "ReluLayer::Forward() - Top and bottom blob shapes do not match";
		return; // TODO return error?
	}

	const float* bottomData = bottom->data_;
	float* topData = top->data_;
	const int count = bottom->count();
	for (int dataIndex = 0; dataIndex < count; ++dataIndex)
	{
		topData[dataIndex] = std::max(bottomData[dataIndex], (float)0);
	}
}

bool ReluTest()
{
	Logger::GetLogger()->LogMessage("ReLU Layer Test:");

	int num = 1, channels = 1, height = 2, width = 4;
	int count = num * channels * height * width;

	Blob bottomBlob(num, channels, height, width);
	Blob topBlob(num, channels, height, width);

	// set input data
	const float dataIn[count] = {1,-1,2,-2,3,-3,4,-4};
	bottomBlob.SetData(dataIn,count);

	// initialise relu layer
	ReluLayer relu1("relu_test", "test_in", "test_out");

	relu1.Forward(&bottomBlob, &topBlob);

	// check result
	bool testPassed = true;
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		bool testPassed_temp;
		if(bottomBlob.data_[dataIndex] > 0)
		{
			testPassed_temp = topBlob.data_[dataIndex] == bottomBlob.data_[dataIndex];
		}
		else
		{
			testPassed_temp = topBlob.data_[dataIndex] == 0;
		}
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError("ReluTest", "ReLU output incorrect at index: %i - input: %d output: %d", dataIndex, bottomBlob.data_[dataIndex], topBlob.data_[dataIndex]);
		}
		testPassed &= testPassed_temp; // include test in overall test result
	}

	std::string resultString = "\tReLU Layer Test ";
	resultString += (testPassed ? "PASSED" : "FAILED");
	std::cout << resultString << std::endl;
	Logger::GetLogger()->LogMessage(resultString);
	return testPassed;
}

}


