#include "dropout_layer.hpp"

#include <cstring>

#include "../blob.hpp"
#include "../logger.hpp"

namespace spaceBrain
{

DropoutLayer::DropoutLayer(std::string name, std::string bottom, std::string top)
{
	DropoutLayer::name_ = name;
	DropoutLayer::bottom_ = bottom;
	DropoutLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tDropout layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void DropoutLayer::Reshape(const Blob* bottom, Blob* top)
{
	top->ReshapeLike(*bottom);
}

void DropoutLayer::Forward(const Blob *bottom, const Blob *top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	float* bottom_data = bottom->getMutableData();
	float* top_data = top->getMutableData();

	memcpy(bottom_data, top_data, sizeof(float) * bottom->count());

	Logger::GetLogger()->LogMessage("\tDropout Layer is only needed for training, this is a dummy layer", name_.c_str());
}

bool DropoutTest()
{
	Logger::GetLogger()->LogMessage("Dropout Layer Test:");

	int num = 2, channels = 3, height = 2, width = 4;
	int count = num * channels * height * width;

	DropoutLayer drop1("dropout_test", "test_in", "test_out"); // initialise dropout layer
	Blob bottomBlob(num, channels, height, width);
	Blob topBlob;

	drop1.SetUp(&bottomBlob, &topBlob);

	// set input data
	float *dataIn = new float[count];
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex * -1^dataIndex;
	}
	bottomBlob.SetData(dataIn,count);

	drop1.Forward(&bottomBlob, &topBlob); // perform forward computation

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
					"DropoutTest",
					"Dropout output incorrect at index: %i - input: %d output: %d",
					dataIndex, bottomData[dataIndex], topData[dataIndex]
			);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tDropout Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);
	return testPassed;
}

}
