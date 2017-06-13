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

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int>* top);

	virtual inline const int type() const
	{
		return RELU;
	}

};

bool ReluTest();

}

#endif /* SRC_LAYERS_RELU_LAYER_HPP_ */
