#include "conv_layer.hpp"

#include "../logger.hpp"
#include "../util/filler.hpp"

namespace spaceBrain
{

ConvolutionLayer::ConvolutionLayer(std::string name, std::string bottom, std::string top, int kernelSize,  int stride, int pad, int numOutputs)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	// these parameters must not be negative and some must not be zero
	if(kernelSize <= 0)
	{
		Logger::GetLogger()->LogError("ConvolutionLayer::ConvolutionLayer", "kernel size %i <= 0", kernelSize);
		return;
	}
	if(stride <= 0)
	{
		Logger::GetLogger()->LogError("ConvolutionLayer::ConvolutionLayer", "stride %i <= 0", stride);
		return;
	}
	if(pad < 0)
	{
		Logger::GetLogger()->LogError("ConvolutionLayer::ConvolutionLayer", "pad %i < 0", pad);
		return;
	}
	if(numOutputs <= 0)
	{
		Logger::GetLogger()->LogError("ConvolutionLayer::ConvolutionLayer", "num output channels %i <= 0", numOutputs);
		return;
	}

	kernel_size_ = kernelSize;
	stride_ = stride;
	pad_ = pad;
	num_output_channels_ = numOutputs; // number of filters
	num_input_channels_ = 1;

	// 1x1 is a special case where im2col is not needed
	is_1x1_ = (kernel_size_ == 1 && stride_ == 1 && pad_ == 0);

	// initialise and zero fill weights
	weights_.Reshape(num_output_channels_, num_input_channels_, kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);

	Logger::GetLogger()->LogMessage(
			"\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tpad = %i, kernelSize = %i, stride = %i, num outputs = %i, num inputs = %i",
			pad_, kernel_size_, stride_, num_output_channels_, num_input_channels_
	);
}

}
