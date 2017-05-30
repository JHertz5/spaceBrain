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

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float> *top);

	virtual inline const char* type() const
	{
		return "Softmax";
	}

};

bool SoftmaxTest();

}




#endif /* SRC_LAYERS_SOFTMAX_LAYER_HPP_ */
