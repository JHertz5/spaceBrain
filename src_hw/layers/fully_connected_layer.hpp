#ifndef SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_
#define SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "../blob.hpp"
#include "../layer.hpp"

namespace spaceBrain
{

class FullyConnectedLayer : public Layer
{
public:
	FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output);
	virtual ~FullyConnectedLayer(){}

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top);

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "Fully Connected";
	}

	Blob<float> weights_;

private:
	int M_;
	int N_;
	int K_;
};

bool FullyConnectedTest();

}


#endif /* SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_ */
