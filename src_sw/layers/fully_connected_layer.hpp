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

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);
	void Forward_gemm(const Blob<float>* bottom, Blob<float>* top);

	void Convolution(const float* input, const float* weights, float* output);

	virtual inline const char* type() const
	{
		return "Fully Connected";
	}

	Blob<float> weights_;

private:
	int input_num_;
	int input_size_;
	int input_depth_;
	int output_depth_;
	int output_size_;
	int input_volume_;

	bool transpose_; // whether the weights should be transposed when computing
	// non-transpose means computation will be standard matrix muliplication
	// tranpose means computation be will elementwise (Hadamard) multiplication
};

bool FullyConnectedTest();

}


#endif /* SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_ */