/*
 * blob.hpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_BLOB_HPP_
#define SRC_BLOB_HPP_

//#include "sds_lib.h"
#include  "data_memory.hpp"

//#define BLOB_SHAPE_DIMENSIONS 4

enum BlobShapeDimensions
{
	NUM = 0,
	CHANNELS = 1,
	HEIGHT = 2,
	WIDTH = 3,
	BLOB_SHAPE_DIMENSIONS = 4
};

namespace spaceBrain
{

class Blob
{
public:

	DataMemory* data_; // pointer to data // XXX may not end up being float

	Blob(const int num, const int channels, const int height, const int width); // constructor
	~Blob(); // destructor

	void Reshape(const int num, const int channels, const int height, const int width);
	void Reshape(const int shape[BLOB_SHAPE_DIMENSIONS]);

	void SetData(const float* dataIn, const int sizeIn);

	inline int offset(const int n, const int c, const int h, const int w)
	{
		return ((n * channels() + c) * height() + h) * width() + w;
	}

	inline int count() const
	{
		return count_;
	}

	inline const int* shape() const
	{
		return shape_;
	}

	inline int num() const
	{
		return shape_[NUM];
	}

	inline int channels() const
	{
		return shape_[CHANNELS];
	}

	inline int height() const
	{
		return shape_[HEIGHT];
	}

	inline int width() const
	{
		return shape_[WIDTH];
	}

protected:
	int shape_[BLOB_SHAPE_DIMENSIONS]; // stores dimensions of blob shape
	int count_; // stores max index of data

};

void BlobTest();

} // end of namespace spaceBrain

#endif /* SRC_BLOB_HPP_ */
