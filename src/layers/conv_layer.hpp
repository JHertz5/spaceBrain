#ifndef SRC_LAYERS_CONV_LAYER_HPP_
#define SRC_LAYERS_CONV_LAYER_HPP_

#include <string>

#include "../blob.hpp"
#include "../layer.hpp"

namespace spaceBrain
{

class ConvolutionLayer : public Layer
{
public:

	ConvolutionLayer(std::string name, std::string bottom, std::string top);
	virtual ~ConvolutionLayer(){}

	virtual inline const char* type() const
	{
		return "Convolution";
	}


};

}

#endif /* SRC_LAYERS_CONV_LAYER_HPP_ */
