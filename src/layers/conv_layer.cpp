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

	//TODO check > 0
	kernel_size_ = kernelSize;
	stride_ = stride;
	pad_ = pad;
	num_output_channels_ = numOutputs;
	num_input_channels_ = 1;

	// 1x1 is a special case where im2col is not needed
	is_1x1_ = (kernel_size_ == 1 && stride_ == 1 && pad_ == 1);

	weights_.Reshape(num_output_channels_, num_input_channels_, kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);

	Logger::GetLogger()->LogMessage("\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

}
