#ifndef SRC_LAYERS_RELU_LAYER_HPP_
#define SRC_LAYERS_RELU_LAYER_HPP_

#include <string>

#include "../layer.hpp"

namespace spaceBrain
{

class ReluLayer : public Layer
{
public:

	ReluLayer(std::string name, std::string bottom, std::string top);
	virtual ~ReluLayer(){}

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "ReLU";
	}

};

bool ReluTest();

}

#endif /* SRC_LAYERS_RELU_LAYER_HPP_ */
