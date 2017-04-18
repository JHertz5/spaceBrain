/*
 * pooling_layer.cpp
 *
 *  Created on: 13 Apr 2017
 *      Author: jhertz5
 */

#include "pooling_layer.hpp"

#include <cmath>
#include <iostream>

#include "../blob.hpp"
#include "../logger.hpp"

namespace spaceBrain
{

PoolingLayer::PoolingLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;
	pad_ = pad;
	kernelSize_ = kernelSize;
	stride_ = stride;

	pooledSize_ = 0;
	channels_ = 0;
	height_ = 0;
	width_ = 0;

	Logger::GetLogger()->LogMessage("\tPooling layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

void PoolingLayer::LayerSetUp(const Blob* bottom, const Blob* top)
{
	bool paramTestsPassed = true;
	if(kernelSize_ < 2)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError("PoolingLayer::Forward()", "Kernel size %i < 2", kernelSize_);
	}
	if(pad_ < 0)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError("PoolingLayer::Forward()", "Padding size %i < 0", pad_);
	}
	if(stride_ < 1)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError("PoolingLayer::Forward()", "stride %i < 1", stride_);
	}

	bool squareCheckPassed = bottom->height() == bottom->width();
	if(!squareCheckPassed)
	{

		Logger::GetLogger()->LogError("PoolingLayer::Forward()", "Bottom blob shape is not square");
	}

	if(!squareCheckPassed || !paramTestsPassed)
	{
		std::cerr << "PoolingLayer::Forward() Error - check log";
	}
}

void PoolingLayer::Reshape(const Blob* bottom, Blob* top)
{
	channels_ = bottom->channels();
	height_ = bottom->height();
	width_ = bottom->width();

	float pooledSize = ((float)(height_ + 2 * pad_ - kernelSize_) / stride_) + 1;
	if(pooledSize != ceil(pooledSize))
	{
		Logger::GetLogger()->LogError("PoolingLayer::Reshape()", "pooledSize %f is non-integer", pooledSize);
		return;
	}
	if((pooledSize-1) * stride_ > height_ + pad_)
	{
		Logger::GetLogger()->LogError("PoolingLayer::Reshape()", "pool will not fit into output - (%i-1)*%i > %i+%i", pooledSize, stride_, height_, pad_);
		return;
	}

	pooledSize_ = pooledSize;
	Logger::GetLogger()->LogMessage("\tPooled size = %f", pooledSize);

	top->Reshape(bottom->num(), channels_, pooledSize, pooledSize);

}


void PoolingLayer::Forward(const Blob *bottom, const Blob *top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());


	const float* bottomData = bottom->getConstData();
	float* topData = top->getMutableData();

	for (int n = 0; n < bottom->num(); ++n) // batch num loop
	{
		for (int c = 0; c < channels_; ++c) // channel loop
		{
			for (int ph = 0; ph < pooledSize_; ++ph) // pool height loop
			{
				for (int pw = 0; pw < pooledSize_; ++pw) // pool width loop
				{
					// find start and end of kernel
					int hstart = ph * stride_ - pad_;
					int wstart = pw * stride_ - pad_;
					int hend = std::min(hstart + kernelSize_, height_);
					int wend = std::min(wstart + kernelSize_, width_);
					hstart = std::max(hstart, 0);
					wstart = std::max(wstart, 0);

					const int pool_index = ph * pooledSize_ + pw;
					for (int h = hstart; h < hend; ++h) // kernel height loop
					{
						for (int w = wstart; w < wend; ++w) // kernel width loop
						{
							const int index = h * width_ + w;
							if (bottomData[index] > topData[pool_index])
							{
								topData[pool_index] = bottomData[index];
							}
						} // end of kernel width loop
					} // end of kernel height loop
				} // end of pool width loop
			} // end of pool height loop
			// compute offset to get to next channel
			bottomData += bottom->offset(0, 1, 0, 0);
			topData += top->offset(0, 1, 0, 0);
		} // end of channel loop
	} // end of batch num loop




}

bool PoolTest()
{
	Logger::GetLogger()->LogMessage("Pooling Layer Test:");

	int num = 1, channels = 1, height = 7, width = 7;
	int count = num * channels * height * width;
	int stride = 2;
	int pad = 0;
	int kernelSize = 3;

	PoolingLayer pool1("pool_test", "test_in", "test_out", pad, kernelSize, stride); // initialise relu layer
	Blob bottomBlob(num, channels, height, width);
	Blob topBlob(num, channels, height, width);

	pool1.SetUp(&bottomBlob, &topBlob);

	// set input data
	float *dataIn = new float[count];
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex;
	}
	bottomBlob.SetData(dataIn,count);

	pool1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// check results
	bool testPassed = true;
	for(int hIndex = 0; hIndex < topBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < topBlob.width(); wIndex++)
		{
			bool testPassed_temp = topBlob.getDataAt(0, 0, hIndex, wIndex) == bottomBlob.getDataAt(0, 0, (stride*hIndex)+topBlob.height()-1, (stride*wIndex)+topBlob.width()-1);

			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError("PoolTest", "Pool output incorrect at index: %i,%i", hIndex, wIndex);
			}
			testPassed &= testPassed_temp; // AND test into overall test result
		}
	}

	std::string resultString = "\tPooling Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}

}
