#include "filler.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include "../blob.hpp"
#include "../logger.hpp"

void FillConstant(Blob<int>* blob, int value)
{
	FillConstant(blob->getMutableData(), blob->count(), value);
}

void FillConstant(int* data, int dataLength, int value)
{
	Logger::GetLogger()->LogMessage("\tFillConstant: Filling blob with constant %i", value);
	if(dataLength < 0)
	{
		Logger::GetLogger()->LogError(
				"FillConstant",
				"dataLength: dataLength < 0",
				dataLength
		);
	}

	// iterate through array, setting each location to value
	for(int dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		data[dataIndex] = value;
	}
}

void FillUniform(Blob<int>* blob, int min, int max)
{
	FillUniform(blob->getMutableData(), blob->count(), min, max);
}

void FillUniform(int* data, int dataLength, int min, int max)
{
	Logger::GetLogger()->LogMessage("\tFillUniform: Filling blob with uniformly distributed x, %i <= x <= %i", min, max);
	if(min > max)
	{
		Logger::GetLogger()->LogError(
				"FillUniform",
				"min > max, swapping values: old min = %i, old max =  %i",
				min, max
		);
		int tmp = max;
		max = min;
		min = tmp;
	}

	int rangeLength = max + 1 - min; // find range of uniform distribution

	// iterate through array, setting each location to uniformly distributed value
	for(int dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		data[dataIndex] = (rand() % rangeLength) + min;
	}
}

bool FillerTest()
{
	Logger::GetLogger()->LogMessage("Filler Test:");

	//  set up blob
	int num = 1, channels = 1, height = 10, width = 10;
	Blob<int> testBlob(num, channels, height, width);

	bool testPassed = true;

	// perform constant fill on blob
	int value = 10;
	FillConstant(&testBlob, value);

	// test results of constant fill
	for(int hIndex = 0; hIndex< testBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < testBlob.width(); wIndex++)
		{
			int resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";

			// check data
			bool testPassed_temp = (resultData == value);
			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError(
						"FillerTest",
						"Constant value incorrect at hIndex %i, wIndex %i - result = %i, expected = %i",
						hIndex, wIndex, resultData, value
				);
			}
			testPassed &= testPassed_temp; // AND test into overall test result
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// perform uniform fill on blob
	int max = 9, min = 0;
	FillUniform(&testBlob, min, max);

	// test results of constant fill
	for(int hIndex = 0; hIndex< testBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < testBlob.width(); wIndex++)
		{
			int resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";

			// check data
			bool testPassed_temp = (resultData <= max && resultData >= min);
			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError(
						"FillerTest",
						"Uniform value outside limits at hIndex %i, wIndex %i - result = %i, min = %i, max = %i",
						hIndex, wIndex, resultData, min, max
				);
			}
			testPassed &= testPassed_temp; // AND test into overall test result
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// print, log & return result
	std::string resultString = "\tFiller Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);
	return testPassed;

}
