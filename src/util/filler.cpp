#include "filler.hpp"

#include <cmath> // for fmod
#include <cstdlib>

#include "../logger.hpp"

namespace spaceBrain
{

void FillConstant(Blob* blob, float value)
{
	Logger::GetLogger()->LogMessage("\tFilling blob with constant %.1f", value);
	int count = blob->count();
	float *data = new float[count]; // create data array to fill with data

	// iterate through array, setting each location to value
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		data[dataIndex] = value;
	}

	blob->SetData(data, count); // set blob data to data array
}

void FillUniform(Blob* blob, int min, int max)
{
	Logger::GetLogger()->LogMessage("\tFilling blob with uniformly distributed x, %i<=x<=%i", min, max);
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

	int count = blob->count();
	float *data = new float[count]; // create data array to fill with data

	int rangeLength = max + 1 - min; // find range of uniform distribution

	// iterate through array, setting each location to uniformly distributed value
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		data[dataIndex] = (rand() % rangeLength) + min;
	}

	blob->SetData(data, count); // set blob data to data array
}

bool FillerTest()
{
	Logger::GetLogger()->LogMessage("Filler Test:");

	//  set up blob
	int num = 1, channels = 1, height = 10, width = 10;
	Blob testBlob(num, channels, height, width);

	bool testPassed = true;

	// perform constant fill on blob
	float value = 10;
	FillConstant(&testBlob, value);

	// test results of constant fill
	for(int hIndex = 0; hIndex< testBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < testBlob.width(); wIndex++)
		{
			float resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";

			// check data
			bool testPassed_temp = (resultData == value);
			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError(
						"FillerTest",
						"Constant value incorrect at hIndex %i, wIndex %i - result = %.1f, expected = %.1f",
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
			float resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";

			// check data
			bool testPassed_temp = (resultData <= max && resultData >= min);
			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError(
						"FillerTest",
						"Uniform value outside limits at hIndex %i, wIndex %i - result = %.1f, min = %i, max = %i",
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

}
