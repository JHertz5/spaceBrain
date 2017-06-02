#ifndef SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_
#define SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_

#include <string>

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

//	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

//	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "Fully Connected";
	}

	Blob<float> weights_;

private:
	int num_output_;
	int input_volume_;
};

}


#endif /* SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_ */
