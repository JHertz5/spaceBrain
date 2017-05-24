#include "blob.hpp"

#include <stdlib.h>
#include <iostream>

#include "logger.hpp"

namespace spaceBrain
{

Blob::Blob()
{
	count_ = 0;
	data_ = new DataMemory();
	shape_[NUM] = 0;
	shape_[CHANNELS] = 0;
	shape_[HEIGHT] = 0;
	shape_[WIDTH] = 0;
	count_ = 0;

	Logger::GetLogger()->LogMessage("\tBlob constructed with no data");
}

Blob::Blob(const int num, const int channels, const int height, const int width)
{
	count_ = 0;
	data_ = new DataMemory();
	Blob::Reshape(num, channels, height, width);

	Logger::GetLogger()->LogMessage("\tBlob constructed with data shape = (%i*%i*%i*%i)", num, channels, height, width);
}

Blob::~Blob()
{
	if(data_ != NULL)
	{
		delete data_; // XXX change to hardware sds_free
	}
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
		Logger::GetLogger()->LogMessage("\tBlob::Reshape: count %i->%i", count_, count);
		count_ = count;
		data_->InitData(count_ * sizeof(float));
	}
}

void Blob::ReshapeLike(const Blob &thatBlob)
{
	Reshape(thatBlob.shape());
}

void Blob::SetData(const float* dataIn, const int countIn)
{
	data_->SetData((void*) dataIn);
}

const float* Blob::getConstData() const
{
	return (const float*) data_->getConstData();
}

float* Blob::getMutableData() const
{
	return (float*) data_->getMutableData();
}

} // end of namespace spaceBrain
