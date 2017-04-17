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

} // end of namespace spaceBrain
