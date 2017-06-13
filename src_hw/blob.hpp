#ifndef SRC_BLOB_HPP_
#define SRC_BLOB_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "logger.hpp"
#include "data_memory.hpp"

enum BlobShapeAxes
{
	NUM_AXIS = 0,
	DEPTH_AXIS = 1,
	HEIGHT_AXIS = 2,
	WIDTH_AXIS = 3,
	NUM_BLOB_DIMENSIONS = 4
};

template <typename Dtype>
class Blob
{
public:

	Blob();
	Blob(std::string name);
	Blob(const int num, const int channels, const int height, const int width);
	Blob(std::string name, const int num, const int channels, const int height, const int width);
	~Blob(); // destructor

	void Reshape(const int num, const int channels, const int height, const int width);
	void Reshape(const int shape[NUM_BLOB_DIMENSIONS]);
	void ReshapeLike(const Blob<Dtype> &thatBlob);

	void SetData(const Dtype* dataIn, const int sizeIn);
	const Dtype* getConstData() const;
	Dtype* getMutableData() const;
	void CopyFrom(const Blob<Dtype>* source, bool reshape);

	void PrintSlice(const int num = 0, const int channel = 0);
	void PrintShape();

	inline int offset(const int n, const int c, const int h, const int w) const
	{
		return ((n * depth() + c) * height() + h) * width() + w;
	}

	inline Dtype getDataAt(const int n, const int c, const int h, const int w)
	{
		return getConstData()[offset(n,c,h,w)];
	}

	inline int count() const
	{
		return count_;
	}

	inline int count(int startAxis, int endAxis = 4) const {
	    if(startAxis > endAxis)
	    {
	    	Logger::GetLogger()->LogError("Blob::Count", "startAxis %i > endAxis %i", startAxis, endAxis);
	    }
	    if(startAxis < 0 || startAxis > NUM_BLOB_DIMENSIONS)
	    {
	    	Logger::GetLogger()->LogError("Blob::Count", "startAxis %i outside range [0,4]", startAxis);
	    }
	    if(endAxis < 0 || endAxis > NUM_BLOB_DIMENSIONS)
		{
	    	Logger::GetLogger()->LogError("Blob::Count", "endAxis %i outside range [0,4]", endAxis);
		}

	    int count = 1;
	    for (int axisIndex = startAxis; axisIndex < endAxis; axisIndex++)
	    {
	      count *= shape_[axisIndex];
	    }
	    return count;
	  }

	inline const int* shape() const
	{
		return shape_;
	}

	inline int num() const
	{
		return shape_[NUM_AXIS];
	}

	inline int depth() const
	{
		return shape_[DEPTH_AXIS];
	}

	inline int height() const
	{
		return shape_[HEIGHT_AXIS];
	}

	inline int width() const
	{
		return shape_[WIDTH_AXIS];
	}

	inline const std::string name() const
	{
		return name_;
	}

protected:
	DataMemory* data_; // pointer to data // XXX may not end up being float
	int shape_[NUM_BLOB_DIMENSIONS]; // stores dimensions of blob shape
	int count_; // stores max index of data
	std::string name_;

};

#endif /* SRC_BLOB_HPP_ */
