#include "net.hpp"

#include "layers/conv_layer.hpp"
#include "layers/fully_connected_layer.hpp"
#include "layers/pooling_layer.hpp"
#include "layers/relu_layer.hpp"

namespace spaceBrain
{

void Net::Init()
{
	// hard-coded to initialise as VGGnet to save time
	// add layers to layers_ in starting from the back

	int pool_pad = 0;
	int pool_kernelSize = 2;
	int pool_stride = 2;

	int conv_pad = 1;
	int conv_kernelSize = 3;
	int conv_stride = 1;


	// fc8
	FullyConnectedLayer* fc8 = new FullyConnectedLayer("fc8", "fc7", "fc8", 1000);
	layer_names_.push_back(fc8->name());
	layers_.push_back(fc8);

	// fc7
	FullyConnectedLayer* fc7 = new FullyConnectedLayer("fc7", "fc6", "fc7", 4096);
	layer_names_.push_back(fc7->name());
	layers_.push_back(fc7);

	// fc6
	FullyConnectedLayer* fc6 = new FullyConnectedLayer("fc6", "pool5", "fc6", 4096);
	layer_names_.push_back(fc6->name());
	layers_.push_back(fc6);

	// pool5
	PoolingLayer* pool5 = new PoolingLayer("pool5", "relu5_3", "pool5", pool_pad, pool_kernelSize, pool_stride);
	layer_names_.push_back(pool5->name());
	layers_.push_back(pool5);

	// relu5_3
	ReluLayer* relu5_3 = new ReluLayer("relu5_3", "conv5_3", "relu5_3");
	layer_names_.push_back(relu5_3->name());
	layers_.push_back(relu5_3);

	// conv5_3
	ConvolutionLayer* conv5_3 = new ConvolutionLayer("conv5_3", "relu5_2", "conv5_3", conv_pad, conv_kernelSize, conv_stride, 512);
	layer_names_.push_back(conv5_3->name());
	layers_.push_back(conv5_3);

	// relu5_2
	ReluLayer* relu5_2 = new ReluLayer("relu5_2", "conv5_2", "relu5_2");
	layer_names_.push_back(relu5_2->name());
	layers_.push_back(relu5_2);

	// conv5_2
	ConvolutionLayer* conv5_2 = new ConvolutionLayer("conv5_2", "relu5_1", "conv5_2", conv_pad, conv_kernelSize, conv_stride, 512);
	layer_names_.push_back(conv5_2->name());
	layers_.push_back(conv5_2);

	// relu5_1
	ReluLayer* relu5_1 = new ReluLayer("relu5_1", "conv5_1", "relu5_1");
	layer_names_.push_back(relu5_1->name());
	layers_.push_back(relu5_1);

	// conv5_1
	ConvolutionLayer* conv5_1 = new ConvolutionLayer("conv5_1", "pool4", "conv5_1", conv_pad, conv_kernelSize, conv_stride, 512);
	layer_names_.push_back(conv5_1->name());
	layers_.push_back(conv5_1);
}

}
