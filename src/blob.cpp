/*
 * blob.cpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#include "blob.hpp"

#include <stdlib.h>
#include <iostream>

#include "logger.hpp"

namespace spaceBrain
{

Blob::Blob(const int num, const int channels, const int height, const int width)
{
	count_ = 0;
	data_ = new DataMemory();
	Blob::Reshape(num, channels, height, width);

	Logger::GetLogger()->LogMessage("\tBlob constructed with data shape = (%i*%i*%i*%i)", num, channels, height, width);
}

Blob::~Blob()
{
	delete data_; // XXX change to hardware sds_free
}

void Blob::Reshape(const int num, const int channels, const int height, const int width)
{
	int shape[BLOB_SHAPE_DIMENSIONS];
	shape[NUM] = num;
	shape[CHANNELS] = channels;
	shape[HEIGHT] = height;
	shape[WIDTH] = width;
	Reshape(shape);
}

void Blob::Reshape(const int shapeIn[BLOB_SHAPE_DIMENSIONS])
{
	int count = 1;
	for (int dimensionIndex = 0; dimensionIndex < BLOB_SHAPE_DIMENSIONS; dimensionIndex++) {
		count *= shapeIn[dimensionIndex];
		Blob::shape_[dimensionIndex] = shapeIn[dimensionIndex];
	}

	if (count != Blob::count_)
	{
		std::cout << "Warning: Reshape is changing size of blob: " << count_ << "->" << count << std::endl;
		count_ = count;
		data_->InitData(count_ * sizeof(float));
	}
}

void Blob::SetData(const float* dataIn, const int countIn)
{
	data_->SetData((void*) dataIn);
}

void BlobTest()
{
	Blob blob1(1,1,2,4);

	/*
	int num = 8;
	data1.InitData(num*sizeof(int));
	int* mut_dataPtr = (int*) data1.getMutableData();

	size_t dataLength = data1.size()/sizeof(int);
	for(size_t dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		mut_dataPtr[dataIndex] = dataIndex;//array[dataIndex];
	}

	std::cout << "data set" << std::endl;

	const int* dataPtr = (const int*) data1.getConstData();

	std::cout << "\tData size = " << dataLength << std::endl;
	std::cout << "\tData contents = { ";
	for(size_t dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		std::cout << dataPtr[dataIndex] << " ";
	}
	std::cout << "}" << std::endl;
	*/
}

} // end of namespace spaceBrain
