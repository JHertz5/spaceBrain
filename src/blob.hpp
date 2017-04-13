/*
 * blob.hpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_BLOB_HPP_
#define SRC_BLOB_HPP_

//#include "sds_lib.h"
#include <stdlib.h>

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

	float *data;  // XXX may not end up being float

	Blob(const uint num, const uint channels, const uint height, const uint width); // constructor
	~Blob(); // destructor

	void Reshape(const uint num, const uint channels, const uint height, const uint width);
	void Reshape(const uint shape[BLOB_SHAPE_DIMENSIONS]);

	void SetData(const float* dataIn, const uint countIn);

	inline uint GetPositionIndex(const uint num, const uint channels, const uint height, const uint width)
	{
		return (((num * shape_[CHANNELS] + channels) * shape_[HEIGHT] + height) * shape_[WIDTH] + width);
	}

	inline uint count() const
	{
		return count_;
	}

	inline const uint* shape() const
	{
		return shape_;
	}

protected:
	uint shape_[BLOB_SHAPE_DIMENSIONS]; // stores dimensions of blob shape
	uint count_; // stores max index of data
};

}

#endif /* SRC_BLOB_HPP_ */
