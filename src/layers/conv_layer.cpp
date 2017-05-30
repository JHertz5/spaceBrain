#include "conv_layer.hpp"

#include "../logger.hpp"

namespace spaceBrain
{

ConvolutionLayer::ConvolutionLayer(std::string name, std::string bottom, std::string top)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	Logger::GetLogger()->LogMessage("\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'", name.c_str(), bottom.c_str(), top.c_str());
}

}
