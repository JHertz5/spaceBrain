#ifndef SRC_LAYERS_DROPOUT_LAYER_HPP_
#define SRC_LAYERS_DROPOUT_LAYER_HPP_

#include <string>

#include "../layer.hpp"

namespace spaceBrain
{

class DropoutLayer : public Layer
{
public:

	DropoutLayer(std::string name, std::string bottom, std::string top);
	virtual ~DropoutLayer(){}

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int>* top);

	virtual inline const int type() const
	{
		return DROPOUT;
	}

};

bool DropoutTest();

}

#endif /* SRC_LAYERS_DROPOUT_LAYER_HPP_ */
