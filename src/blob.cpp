/*
 * blob.cpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#include "blob.hpp"
#include <iostream>

namespace spaceBrain
{

Blob::Blob(const uint num, const uint channels, const uint height, const uint width)
{
	count_ = num * channels * height * width;
	data = (float *)malloc(count_ * sizeof(float)); // TODO change to hardware sds_alloc
	Blob::Reshape(num, channels, height, width);
}

Blob::~Blob()
{
	free(data); // TODO change to hardware sds_free
}

void Blob::Reshape(const uint num, const uint channels, const uint height, const uint width)
{
	uint shape[BLOB_SHAPE_DIMENSIONS];
	shape[NUM] = num;
	shape[CHANNELS] = channels;
	shape[HEIGHT] = height;
	shape[WIDTH] = width;
	Reshape(shape);
}

void Blob::Reshape(const uint shapeIn[BLOB_SHAPE_DIMENSIONS])
{
	uint count = 1;
	for (int dimensionIndex = 0; dimensionIndex < BLOB_SHAPE_DIMENSIONS; dimensionIndex++) {
		count *= shapeIn[dimensionIndex];
		Blob::shape_[dimensionIndex] = shapeIn[dimensionIndex];
	}
	if (count != Blob::count_)
	{
		std::cout << "Warning: Reshape change is changing size of blob: " << count << "->" << count << std::endl;
	}
}

void Blob::SetData(const float* dataIn, const uint countIn)
{
	for(uint dataIndex = 0; dataIndex < count_; dataIndex++)
	{
		if(dataIndex < countIn)
		{
			data[dataIndex] = dataIn[dataIndex];
		}
		else
		{
			data[dataIndex] = (float)0;
		}
	}
}

}
