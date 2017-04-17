/*
 * pooling_layer.cpp
 *
 *  Created on: 13 Apr 2017
 *      Author: jhertz5
 */

#include "pooling_layer.hpp"

#include "../logger.hpp"

namespace spaceBrain
{

PoolingLayer::PoolingLayer(std::string name, std::string bottom, std::string top)
{
	PoolingLayer::name_ = name;
	PoolingLayer::bottom_ = bottom;
	PoolingLayer::top_ = top;
	Logger::GetLogger()->LogMessage("\tPooling layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void PoolingLayer::Forward(const Blob *bottom, const Blob *top)
{
	Logger::GetLogger()->LogMessage("Pooling Forward not implemented");
/*
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	bool sizeCheckPassed = true;
	for(int dimensionIndex = 0; dimensionIndex < BLOB_SHAPE_DIMENSIONS; dimensionIndex++)
	{
		sizeCheckPassed &= bottom->shape()[dimensionIndex] == top->shape()[dimensionIndex];
	}
	if(!sizeCheckPassed)
	{

		Logger::GetLogger()->LogError("PoolingLayer::Forward()", "Top and bottom blob shapes do not match");
		std::cerr << "PoolingLayer::Forward() - Top and bottom blob shapes do not match";
		return; // TODO return error?
	}

	const float* bottomData = bottom->data;
	float* topData = top->data;
	const int count = bottom->count();
	for (int dataIndex = 0; dataIndex < count; ++dataIndex)
	{
		topData[dataIndex] = std::max(bottomData[dataIndex], (float)0);
	}
	*/
}

bool PoolingTest()
{
	Logger::GetLogger()->LogMessage("Pooling Layer Test:");
	bool testPassed = false;

/*
	int num = 1, channels = 1, height = 2, width = 4;
	int count = num * channels * height * width;

	Blob bottomBlob(num, channels, height, width);
	Blob topBlob(num, channels, height, width);

	// set input data
	const float dataIn[count] = {1,-1,2,-2,3,-3,4,-4};
	bottomBlob.SetData(dataIn,count);

	// initialise pooling layer
	PoolingLayer pooling1("pooling_test", "test_in", "test_out");

	pooling1.Forward(&bottomBlob, &topBlob);

	// check result
	bool testPassed = true;
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		bool testPassed_temp;
		if(bottomBlob.data[dataIndex] > 0)
		{
			testPassed_temp = topBlob.data[dataIndex] == bottomBlob.data[dataIndex];
		}
		else
		{
			testPassed_temp = topBlob.data[dataIndex] == 0;
		}
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError("PoolingTest", "Pooling output incorrect at index: %i - input: %d output: %d", dataIndex, bottomBlob.data[dataIndex], topBlob.data[dataIndex]);
		}
		testPassed &= testPassed_temp; // include test in overall test result
	}
*/
	/*
	std::string resultString = "\tPooling Layer Test ";
	resultString += (testPassed ? "PASSED" : "FAILED");
	std::cout << resultString << std::endl;
	Logger::GetLogger()->LogMessage(resultString);
	*/

	Logger::GetLogger()->LogMessage("PoolingTest() not implemented");

	return testPassed;
}

}




