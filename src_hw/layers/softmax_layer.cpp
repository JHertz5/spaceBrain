#include <blob.hpp>
#include <layers/softmax_layer.hpp>
#include <logger.hpp>
#include <iostream>
#include <string>

namespace spaceBrain
{

SoftmaxLayer::SoftmaxLayer(std::string name, std::string bottom, std::string top)
{
	SoftmaxLayer::name_ = name;
	SoftmaxLayer::bottom_ = bottom;
	SoftmaxLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tSoftmax layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void SoftmaxLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	top->ReshapeLike(*bottom);
}

void SoftmaxLayer::Forward(const Blob<float>* bottom, Blob<float>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	top->CopyFrom(bottom, true);

	std::string softmaxDummyLayerMessage = "\tSOFTMAX FORWARD() IS NOT YET IMPLEMENTED, THIS IS A DUMMY LAYER";
	Logger::GetLogger()->LogMessage(softmaxDummyLayerMessage);
	std::cout << softmaxDummyLayerMessage << std::endl;
}

bool SoftmaxTest() // TODO test proper version
{
	Logger::GetLogger()->LogMessage("Softmax Layer Test:");

	int num = 2, channels = 3, height = 2, width = 4;
	int count = num * channels * height * width;

	SoftmaxLayer prob1("prob_test", "test_in", "test_out"); // initialise dropout layer
	Blob<float> bottomBlob(num, channels, height, width);
	Blob<float> topBlob;

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
