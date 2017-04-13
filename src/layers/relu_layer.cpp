/*
 * relu_layer.cpp
 *
 *  Created on: 11 Apr 2017
 *      Author: jjh113
 */

#include "relu_layer.hpp"
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
}

void ReluLayer::Forward(const Blob &bottom, const Blob &top)
{
	if(*bottom.shape() != *top.shape())
	{
		std::cout << "not same shape" << std::endl; // TODO do some logging
		return; // TODO maybe return error?
	}

	const float* bottomData = bottom.data;
	float* topData = top.data;
	const uint count = bottom.count();
	for (uint dataIndex = 0; dataIndex < count; ++dataIndex)
	{
		topData[dataIndex] = std::max(bottomData[dataIndex], (float)0);
	}
}

bool ReluTest()
{
	spaceBrain::Blob bottomBlob(1,2,2,2);
	spaceBrain::Blob topBlob(1,2,2,2);

	// set input data
	const float dataIn[2*2*2] = {1,-1,2,-2,3,-3,4,-4};
	bottomBlob.SetData(dataIn,2*2*2);

	// initialise relu layer
	ReluLayer relu1("relu1", "data", "conv1");
	std::cout << "\tReLU layer initialised" << std::endl;
	std::cout << "\t\ttype: " << relu1.type() << ", name: " << relu1.name() << std::endl;

	relu1.Forward(bottomBlob, topBlob);

	bool testPassed = true;

	// print bottom blob data
	std::cout << "\ttop data = \t{ ";
	for(uint dataIndex = 0; dataIndex < 8; dataIndex++)
	{
		std::cout << topBlob.data[dataIndex] << " ";
		if(bottomBlob.data[dataIndex] > 0)
		{
			testPassed &= topBlob.data[dataIndex] == bottomBlob.data[dataIndex];
		}
		else
		{
			testPassed &= topBlob.data[dataIndex] == 0;
		}
	}
	std::cout << "}" << std::endl;

	std::cout << "\tReLU Layer Test " << (testPassed ? "PASSED" : "FAILED") << std::endl << std::endl;
	return testPassed;
}

}


