#include "softmax_layer.hpp"

#include <cstring>
#include <iostream>

#include "../blob.hpp"
#include "../logger.hpp"

namespace spaceBrain
{

SoftmaxLayer::SoftmaxLayer(std::string name, std::string bottom, std::string top)
{
	SoftmaxLayer::name_ = name;
	SoftmaxLayer::bottom_ = bottom;
	SoftmaxLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tSoftmax layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void SoftmaxLayer::Reshape(const Blob* bottom, Blob* top)
{
	top->ReshapeLike(*bottom);
}

// TODO proper version

void SoftmaxLayer::Forward(const Blob *bottom, const Blob *top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	float* bottom_data = bottom->getMutableData();
	float* top_data = top->getMutableData();

	memcpy(bottom_data, top_data, sizeof(float) * bottom->count());

	Logger::GetLogger()->LogMessage("\tSoftmax Forward() is not yet implemented, this is a dummy layer", name_.c_str());
}

bool SoftmaxTest() // TODO test proper version
{
	Logger::GetLogger()->LogMessage("Softmax Layer Test:");

	int num = 2, channels = 3, height = 2, width = 4;
	int count = num * channels * height * width;

	SoftmaxLayer prob1("prob_test", "test_in", "test_out"); // initialise dropout layer
	Blob bottomBlob(num, channels, height, width);
	Blob topBlob;

	prob1.SetUp(&bottomBlob, &topBlob);

	// set input data
	float *dataIn = new float[count];
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex * -1^dataIndex;
	}
	bottomBlob.SetData(dataIn,count);

	prob1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// get results
	const float* bottomData = bottomBlob.getConstData();
	const float* topData = topBlob.getConstData();

	// check results
	bool testPassed = true;
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		bool testPassed_temp;
		testPassed_temp = topData[dataIndex] == bottomData[dataIndex];
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"SoftmaxTest",
					"Softmax output incorrect at index: %i - input: %d output: %d",
					dataIndex, bottomData[dataIndex], topData[dataIndex]
			);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tSoftmax Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);
	return testPassed;
}

}
