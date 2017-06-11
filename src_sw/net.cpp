#include "net.hpp"

#include <stddef.h>
#include <iostream>

#include "layers/conv_layer.hpp"
#include "layers/fully_connected_layer.hpp"
#include "layers/pooling_layer.hpp"
#include "layers/relu_layer.hpp"

namespace spaceBrain
{

void Net::Init(std::string name)
{
	name_ = name;

	SetLayers();
}

void Net::SetLayers()
{
	// hard-coded to initialise as VGGnet to save time
	// add layers to layers_ in starting from the back

	const int pool_pad = 0;
	const int pool_kernelSize = 2;
	const int pool_stride = 2;

	const int conv_pad = 1;
	const int conv_kernelSize = 3;
	const int conv_stride = 1;

	const int num_output_5 = 512;
	const int num_output_4 = 512;
	const int num_output_3 = 256;
	const int num_output_2 = 128;
	const int num_output_1 = 64;

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
	ConvolutionLayer* conv5_3 = new ConvolutionLayer("conv5_3", "relu5_2", "conv5_3", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layer_names_.push_back(conv5_3->name());
	layers_.push_back(conv5_3);

	// relu5_2
	ReluLayer* relu5_2 = new ReluLayer("relu5_2", "conv5_2", "relu5_2");
	layer_names_.push_back(relu5_2->name());
	layers_.push_back(relu5_2);

	// conv5_2
	ConvolutionLayer* conv5_2 = new ConvolutionLayer("conv5_2", "relu5_1", "conv5_2", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layer_names_.push_back(conv5_2->name());
	layers_.push_back(conv5_2);

	// relu5_1
	ReluLayer* relu5_1 = new ReluLayer("relu5_1", "conv5_1", "relu5_1");
	layer_names_.push_back(relu5_1->name());
	layers_.push_back(relu5_1);

	// conv5_1
	ConvolutionLayer* conv5_1 = new ConvolutionLayer("conv5_1", "pool4", "conv5_1", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layer_names_.push_back(conv5_1->name());
	layers_.push_back(conv5_1);

	// pool4
	PoolingLayer* pool4 = new PoolingLayer("pool4", "relu4_3", "pool4", pool_pad, pool_kernelSize, pool_stride);
	layer_names_.push_back(pool4->name());
	layers_.push_back(pool4);

	// relu4_3
	ReluLayer* relu4_3 = new ReluLayer("relu4_3", "conv4_3", "relu4_3");
	layer_names_.push_back(relu4_3->name());
	layers_.push_back(relu4_3);

	// conv4_3
	ConvolutionLayer* conv4_3 = new ConvolutionLayer("conv4_3", "relu4_2", "conv4_3", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layer_names_.push_back(conv4_3->name());
	layers_.push_back(conv4_3);

	// relu4_2
	ReluLayer* relu4_2 = new ReluLayer("relu4_2", "conv4_2", "relu4_2");
	layer_names_.push_back(relu4_2->name());
	layers_.push_back(relu4_2);

	// conv4_2
	ConvolutionLayer* conv4_2 = new ConvolutionLayer("conv4_2", "relu4_1", "conv4_2", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layer_names_.push_back(conv4_2->name());
	layers_.push_back(conv4_2);

	// relu4_1
	ReluLayer* relu4_1 = new ReluLayer("relu4_1", "conv4_1", "relu4_1");
	layer_names_.push_back(relu4_1->name());
	layers_.push_back(relu4_1);

	// conv4_1
	ConvolutionLayer* conv4_1 = new ConvolutionLayer("conv4_1", "pool3", "conv4_1", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layer_names_.push_back(conv4_1->name());
	layers_.push_back(conv4_1);

	// pool3
	PoolingLayer* pool3 = new PoolingLayer("pool3", "relu3_3", "pool3", pool_pad, pool_kernelSize, pool_stride);
	layer_names_.push_back(pool3->name());
	layers_.push_back(pool3);

	// relu3_3
	ReluLayer* relu3_3 = new ReluLayer("relu3_3", "conv3_3", "relu3_3");
	layer_names_.push_back(relu3_3->name());
	layers_.push_back(relu3_3);

	// conv3_3
	ConvolutionLayer* conv3_3 = new ConvolutionLayer("conv3_3", "relu3_2", "conv3_3", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layer_names_.push_back(conv3_3->name());
	layers_.push_back(conv3_3);

	// relu3_2
	ReluLayer* relu3_2 = new ReluLayer("relu3_2", "conv3_2", "relu3_2");
	layer_names_.push_back(relu3_2->name());
	layers_.push_back(relu3_2);

	// conv3_2
	ConvolutionLayer* conv3_2 = new ConvolutionLayer("conv3_2", "relu3_1", "conv3_2", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layer_names_.push_back(conv3_2->name());
	layers_.push_back(conv3_2);

	// relu3_1
	ReluLayer* relu3_1 = new ReluLayer("relu3_1", "conv3_1", "relu3_1");
	layer_names_.push_back(relu3_1->name());
	layers_.push_back(relu3_1);

	// conv3_1
	ConvolutionLayer* conv3_1 = new ConvolutionLayer("conv3_1", "pool2", "conv3_1", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layer_names_.push_back(conv3_1->name());
	layers_.push_back(conv3_1);

	// pool2
	PoolingLayer* pool2 = new PoolingLayer("pool2", "relu2_2", "pool2", pool_pad, pool_kernelSize, pool_stride);
	layer_names_.push_back(pool2->name());
	layers_.push_back(pool2);

	// relu2_2
	ReluLayer* relu2_2 = new ReluLayer("relu2_2", "conv2_2", "relu2_2");
	layer_names_.push_back(relu2_2->name());
	layers_.push_back(relu2_2);

	// conv2_2
	ConvolutionLayer* conv2_2 = new ConvolutionLayer("conv2_2", "relu2_1", "conv2_2", conv_pad, conv_kernelSize, conv_stride, num_output_2);
	layer_names_.push_back(conv2_2->name());
	layers_.push_back(conv2_2);

	// relu2_1
	ReluLayer* relu2_1 = new ReluLayer("relu2_1", "conv2_1", "relu2_1");
	layer_names_.push_back(relu2_1->name());
	layers_.push_back(relu2_1);

	// conv2_1
	ConvolutionLayer* conv2_1 = new ConvolutionLayer("conv2_1", "pool1", "conv2_1", conv_pad, conv_kernelSize, conv_stride, num_output_2);
	layer_names_.push_back(conv2_1->name());
	layers_.push_back(conv2_1);

	// pool1
	PoolingLayer* pool1 = new PoolingLayer("pool1", "relu1_2", "pool1", pool_pad, pool_kernelSize, pool_stride);
	layer_names_.push_back(pool1->name());
	layers_.push_back(pool1);

	// relu1_2
	ReluLayer* relu1_2 = new ReluLayer("relu1_2", "conv1_2", "relu1_2");
	layer_names_.push_back(relu1_2->name());
	layers_.push_back(relu1_2);

	// conv1_2
	ConvolutionLayer* conv1_2 = new ConvolutionLayer("conv1_2", "relu1_1", "conv1_2", conv_pad, conv_kernelSize, conv_stride, num_output_1);
	layer_names_.push_back(conv1_2->name());
	layers_.push_back(conv1_2);

	// relu1_1
	ReluLayer* relu1_1 = new ReluLayer("relu1_1", "conv1_1", "relu1_1");
	layer_names_.push_back(relu1_1->name());
	layers_.push_back(relu1_1);

	// conv1_1
	ConvolutionLayer* conv1_1 = new ConvolutionLayer("conv1_1", "data", "conv1_1", conv_pad, conv_kernelSize, conv_stride, num_output_1);
	layer_names_.push_back(conv1_1->name());
	layers_.push_back(conv1_1);

	// input layer
//	input: "data"
//	input_dim: 1
//	input_dim: 3
//	input_dim: 224
//	input_dim: 224

	for(size_t index = 0; index < layers_.size(); index++)
	{
		std::cout << index << "\t" << layer_names_[index] << std::endl;
	}

	std::cout << fc8->name() << std::endl;

}

void NetTest()
{
	Net net1;
	net1.Init("testNet");
}

}
