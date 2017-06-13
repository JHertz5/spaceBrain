#ifndef SRC_LAYERS_DROPOUT_LAYER_HPP_
#define SRC_LAYERS_DROPOUT_LAYER_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "../blob.hpp"
#include "../layer.hpp"

class DropoutLayer : public Layer
{
public:

	DropoutLayer(std::string name, std::string bottom, std::string top);
	virtual ~DropoutLayer(){}

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const int type() const
	{
		return DROPOUT;
	}

};

bool DropoutTest();

#endif /* SRC_LAYERS_DROPOUT_LAYER_HPP_ */
