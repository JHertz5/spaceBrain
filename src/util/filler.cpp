#include "filler.hpp"

#include <cmath> // for fmod
#include <cstdlib>

#include "../logger.hpp"

namespace spaceBrain
{

void FillConstant(Blob* blob, float value)
{
	int count = blob->count();
	float *data = new float[count];

	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		data[dataIndex] = value;
	}

	blob->SetData(data, count);
}

void FillUniform(Blob* blob, int min, int max)
{
	//TODO check <
	int count = blob->count();
	float *data = new float[count];

	int rangeLength = max + 1 - min;

	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		data[dataIndex] = (rand() % rangeLength) + min;
	}

	blob->SetData(data, count);
}

bool FillerTest()
{
	Logger::GetLogger()->LogMessage("Filler Test:");

	int num = 1, channels = 1, height = 50, width = 50;
	Blob testBlob(num, channels, height, width);

	bool testPassed = true;

	float value = 10;
	FillConstant(&testBlob, value);

	for(int hIndex = 0; hIndex< testBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < testBlob.width(); wIndex++)
		{
			float resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";
			testPassed &= (resultData == value);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	int max = 9, min = 0;
	FillUniform(&testBlob, min, max);

	for(int hIndex = 0; hIndex< testBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < testBlob.width(); wIndex++)
		{
			float resultData = testBlob.getDataAt(0, 0, hIndex, wIndex);
			std::cout << resultData << "\t";
			testPassed &= (resultData <= max && resultData >= min);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return testPassed;

}

}
