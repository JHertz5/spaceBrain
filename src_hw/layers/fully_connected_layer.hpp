#ifndef SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_
#define SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "../blob.hpp"
#include "../layer.hpp"

class FullyConnectedLayer : public Layer
{
public:
	FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output, bool transpose);
	virtual ~FullyConnectedLayer(){}

	virtual void LayerSetUp(const Blob<int>* bottom, const Blob<int>* top);

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int>* top);
	void Forward_gemm(const Blob<int>* bottom, Blob<int>* top);

	void Convolution(const int* input, const int* weights, int* output);

	virtual inline const int type() const
	{
		return FC;
	}

	Blob<int> weights_;

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

bool FullyConnectedCompare();

#endif /* SRC_LAYERS_FULLY_CONNECTED_LAYER_HPP_ */
