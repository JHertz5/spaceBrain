#ifndef SRC_BLOB_HPP_
#define SRC_BLOB_HPP_

//#include "sds_lib.h"
#include  "data_memory.hpp"

enum BlobShapeDimensions
{
	NUM_LAYERS = 0,
	CHANNELS = 1,
	HEIGHT = 2,
	WIDTH = 3,
	NUM_BLOB_DIMENSIONS = 4
};

namespace spaceBrain
{

template <typename Dtype>
class Blob
{
public:

	Blob();
	Blob(const int num, const int channels, const int height, const int width); // constructor
	~Blob(); // destructor

	void Reshape(const int num, const int channels, const int height, const int width);
	void Reshape(const int shape[NUM_BLOB_DIMENSIONS]);
	void ReshapeLike(const Blob<Dtype> &thatBlob);

	void SetData(const Dtype* dataIn, const int sizeIn);
	const Dtype* getConstData() const;
	Dtype* getMutableData() const;
	void CopyFrom(const Blob<Dtype>* source, bool reshape);

	void PrintSlice(const int num = 0, const int channel = 0);

	inline int offset(const int n, const int c, const int h, const int w) const
	{
		return ((n * channels() + c) * height() + h) * width() + w;
	}

	inline Dtype getDataAt(const int n, const int c, const int h, const int w)
	{
		return getConstData()[offset(n,c,h,w)];
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
		return shape_[NUM_LAYERS];
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
	DataMemory* data_; // pointer to data // XXX may not end up being float
	int shape_[NUM_BLOB_DIMENSIONS]; // stores dimensions of blob shape
	int count_; // stores max index of data

};

} // end of namespace spaceBrain

#endif /* SRC_BLOB_HPP_ */
