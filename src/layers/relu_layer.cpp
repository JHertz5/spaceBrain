#include "relu_layer.hpp"

#include <algorithm>
#include <iostream>

#include "../blob.hpp"
#include "../logger.hpp"

namespace spaceBrain
{

ReluLayer::ReluLayer(std::string name, std::string bottom, std::string top)
{
	ReluLayer::name_ = name;
	ReluLayer::bottom_ = bottom;
	ReluLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tReLU layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void ReluLayer::Reshape(const Blob* bottom, Blob* top)
{
	top->ReshapeLike(*bottom);
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

	const float* bottomData = bottom->getConstData();
	float* topData = top->getMutableData();
	const int count = bottom->count();
	for (int dataIndex = 0; dataIndex < count; ++dataIndex)
	{
		topData[dataIndex] = std::max(bottomData[dataIndex], (float)0);
	}
}

bool ReluTest()
{
	Logger::GetLogger()->LogMessage("ReLU Layer Test:");

	int num = 2, channels = 3, height = 2, width = 4;
	int count = num * channels * height * width;

	ReluLayer relu1("relu_test", "test_in", "test_out"); // initialise relu layer
	Blob bottomBlob(num, channels, height, width);
	Blob topBlob;

	relu1.SetUp(&bottomBlob, &topBlob);

	// set input data
	float *dataIn = new float[count];
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex * -1^dataIndex;
	}
	bottomBlob.SetData(dataIn,count);

	relu1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// get results
	const float* bottomData = bottomBlob.getConstData();
	const float* topData = topBlob.getConstData();

	// check results
	bool testPassed = true;
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		bool testPassed_temp;
		if(bottomData[dataIndex] > 0)
		{
			testPassed_temp = topData[dataIndex] == bottomData[dataIndex];
		}
		else
		{
			testPassed_temp = topData[dataIndex] == 0;
		}
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError("ReluTest", "ReLU output incorrect at index: %i - input: %d output: %d", dataIndex, bottomData[dataIndex], topData[dataIndex]);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tReLU Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);
	return testPassed;
}

}


