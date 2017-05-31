#include "conv_layer.hpp"

#include <cmath>
#include <iostream>

#include "../logger.hpp"
#include "../util/filler.hpp"

namespace spaceBrain
{

ConvolutionLayer::ConvolutionLayer(std::string name, std::string bottom, std::string top, int kernelSize,  int stride, int pad, int numOutputs)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	kernel_size_ = kernelSize;
	stride_ = stride;
	pad_ = pad;
	num_output_channels_ = numOutputs; // number of filters
	num_input_channels_ = 1;

	conved_size_ = 0;
	channels_ = 0;
	input_size_ = 0;

	is_1x1_ = false;

	Logger::GetLogger()->LogMessage(
			"\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tpad = %i, kernelSize = %i, stride = %i, num outputs = %i, num inputs = %i",
			pad_, kernel_size_, stride_, num_output_channels_, num_input_channels_
	);
}

void ConvolutionLayer::LayerSetUp(const Blob<float>* bottom, const Blob<float>* top)
{
	// check that each parameter is valid
	bool paramTestsPassed = true;
	if(kernel_size_ < 2)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Kernel size %i < 2",
				kernel_size_
		);
	}
	if(pad_ < 0)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Padding size %i < 0",
				pad_
		);
	}
	if(stride_ < 1)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"stride %i < 1",
				stride_
		);
	}

	// check that bottom blob is square
	bool squareCheckPassed = bottom->height() == bottom->width();
	if(!squareCheckPassed)
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Bottom blob shape is not square"
		);
	}

	if(!squareCheckPassed || !paramTestsPassed)
	{
		std::cerr << "ConvolutionLayer::LayerSetUp Error - check log";
	}

	// 1x1 is a special case where im2col is not needed
	is_1x1_ = (kernel_size_ == 1 && stride_ == 1 && pad_ == 0);

	// initialise and zero fill weights
	weights_.Reshape(num_output_channels_, num_input_channels_, kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);
}

void ConvolutionLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	channels_ = bottom->channels();
	input_size_ = bottom->height();
	input_size_ = bottom->width();

	int inputSize = input_size_;

	float convedSize = ((float)(inputSize + 2 * pad_ - kernel_size_) / stride_) + 1;
	if(convedSize != ceil(convedSize))
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"convedSize %f is non-integer",
				convedSize
		);
		return;
	}
	if((convedSize-1) * stride_ > inputSize + pad_)
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"conv will not fit into output - (%i-1)*%i > %i+%i",
				convedSize, stride_, inputSize, pad_
		);
		return;
	}

	conved_size_ = convedSize;
	Logger::GetLogger()->LogMessage("\tConvolved size = %f", convedSize);

	top->Reshape(bottom->num(), channels_, convedSize, convedSize);
}

}
