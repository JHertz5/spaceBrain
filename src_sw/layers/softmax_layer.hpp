#ifndef SRC_LAYERS_SOFTMAX_LAYER_HPP_
#define SRC_LAYERS_SOFTMAX_LAYER_HPP_

#include <string>

#include "../layer.hpp"

namespace spaceBrain
{

class SoftmaxLayer : public Layer
{
public:

	SoftmaxLayer(std::string name, std::string bottom, std::string top);
	virtual ~SoftmaxLayer(){}

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int> *top);

	virtual inline const int type() const
	{
		return SOFTMAX;
	}

};

bool SoftmaxTest();

}

#endif /* SRC_LAYERS_SOFTMAX_LAYER_HPP_ */
