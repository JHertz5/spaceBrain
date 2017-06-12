#include "net.hpp"

#include <stddef.h>
#include <iostream>

#include "layers/conv_layer.hpp"
#include "layers/fully_connected_layer.hpp"
#include "layers/pooling_layer.hpp"
#include "layers/relu_layer.hpp"
#include "logger.hpp"
#include "util/filler.hpp"

namespace spaceBrain
{

void Net::Init()
{
	SetLayers();
	SetBlobs();
	Reshape();
	PrintLayerOutputShapes();
}


void Net::SetLayers()
{
	Logger::GetLogger()->LogMessage("\nNet::SetLayers(): Setting hard-coded VGGnet");
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

	const bool fc_transpose = false;

	// conv1_1
	ConvolutionLayer* conv1_1 = new ConvolutionLayer("conv1_1", "data", "conv1_1", conv_pad, conv_kernelSize, conv_stride, num_output_1);
	layers_.push_back(conv1_1);

	// relu1_1
	ReluLayer* relu1_1 = new ReluLayer("relu1_1", "conv1_1", "relu1_1");
	layers_.push_back(relu1_1);

	// conv1_2
	ConvolutionLayer* conv1_2 = new ConvolutionLayer("conv1_2", "relu1_1", "conv1_2", conv_pad, conv_kernelSize, conv_stride, num_output_1);
	layers_.push_back(conv1_2);

	// relu1_2
	ReluLayer* relu1_2 = new ReluLayer("relu1_2", "conv1_2", "relu1_2");
	layers_.push_back(relu1_2);

	// pool1
	PoolingLayer* pool1 = new PoolingLayer("pool1", "relu1_2", "pool1", pool_pad, pool_kernelSize, pool_stride);
	layers_.push_back(pool1);

	// conv2_1
	ConvolutionLayer* conv2_1 = new ConvolutionLayer("conv2_1", "pool1", "conv2_1", conv_pad, conv_kernelSize, conv_stride, num_output_2);
	layers_.push_back(conv2_1);

	// relu2_1
	ReluLayer* relu2_1 = new ReluLayer("relu2_1", "conv2_1", "relu2_1");
	layers_.push_back(relu2_1);

	// conv2_2
	ConvolutionLayer* conv2_2 = new ConvolutionLayer("conv2_2", "relu2_1", "conv2_2", conv_pad, conv_kernelSize, conv_stride, num_output_2);
	layers_.push_back(conv2_2);

	// relu2_2
	ReluLayer* relu2_2 = new ReluLayer("relu2_2", "conv2_2", "relu2_2");
	layers_.push_back(relu2_2);

	// pool2
	PoolingLayer* pool2 = new PoolingLayer("pool2", "relu2_2", "pool2", pool_pad, pool_kernelSize, pool_stride);
	layers_.push_back(pool2);

	// conv3_1
	ConvolutionLayer* conv3_1 = new ConvolutionLayer("conv3_1", "pool2", "conv3_1", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layers_.push_back(conv3_1);

	// relu3_1
	ReluLayer* relu3_1 = new ReluLayer("relu3_1", "conv3_1", "relu3_1");
	layers_.push_back(relu3_1);

	// conv3_2
	ConvolutionLayer* conv3_2 = new ConvolutionLayer("conv3_2", "relu3_1", "conv3_2", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layers_.push_back(conv3_2);

	// relu3_2
	ReluLayer* relu3_2 = new ReluLayer("relu3_2", "conv3_2", "relu3_2");
	layers_.push_back(relu3_2);

	// conv3_3
	ConvolutionLayer* conv3_3 = new ConvolutionLayer("conv3_3", "relu3_2", "conv3_3", conv_pad, conv_kernelSize, conv_stride, num_output_3);
	layers_.push_back(conv3_3);

	// relu3_3
	ReluLayer* relu3_3 = new ReluLayer("relu3_3", "conv3_3", "relu3_3");
	layers_.push_back(relu3_3);

	// pool3
	PoolingLayer* pool3 = new PoolingLayer("pool3", "relu3_3", "pool3", pool_pad, pool_kernelSize, pool_stride);
	layers_.push_back(pool3);

	// conv4_1
	ConvolutionLayer* conv4_1 = new ConvolutionLayer("conv4_1", "pool3", "conv4_1", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layers_.push_back(conv4_1);

	// relu4_1
	ReluLayer* relu4_1 = new ReluLayer("relu4_1", "conv4_1", "relu4_1");
	layers_.push_back(relu4_1);

	// conv4_2
	ConvolutionLayer* conv4_2 = new ConvolutionLayer("conv4_2", "relu4_1", "conv4_2", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layers_.push_back(conv4_2);

	// relu4_2
	ReluLayer* relu4_2 = new ReluLayer("relu4_2", "conv4_2", "relu4_2");
	layers_.push_back(relu4_2);

	// conv4_3
	ConvolutionLayer* conv4_3 = new ConvolutionLayer("conv4_3", "relu4_2", "conv4_3", conv_pad, conv_kernelSize, conv_stride, num_output_4);
	layers_.push_back(conv4_3);

	// relu4_3
	ReluLayer* relu4_3 = new ReluLayer("relu4_3", "conv4_3", "relu4_3");
	layers_.push_back(relu4_3);

	// pool4
	PoolingLayer* pool4 = new PoolingLayer("pool4", "relu4_3", "pool4", pool_pad, pool_kernelSize, pool_stride);
	layers_.push_back(pool4);

	// conv5_1
	ConvolutionLayer* conv5_1 = new ConvolutionLayer("conv5_1", "pool4", "conv5_1", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layers_.push_back(conv5_1);

	// relu5_1
	ReluLayer* relu5_1 = new ReluLayer("relu5_1", "conv5_1", "relu5_1");
	layers_.push_back(relu5_1);

	// conv5_2
	ConvolutionLayer* conv5_2 = new ConvolutionLayer("conv5_2", "relu5_1", "conv5_2", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layers_.push_back(conv5_2);

	// relu5_2
	ReluLayer* relu5_2 = new ReluLayer("relu5_2", "conv5_2", "relu5_2");
	layers_.push_back(relu5_2);

	// conv5_3
	ConvolutionLayer* conv5_3 = new ConvolutionLayer("conv5_3", "relu5_2", "conv5_3", conv_pad, conv_kernelSize, conv_stride, num_output_5);
	layers_.push_back(conv5_3);

	// relu5_3
	ReluLayer* relu5_3 = new ReluLayer("relu5_3", "conv5_3", "relu5_3");
	layers_.push_back(relu5_3);

	// pool5
	PoolingLayer* pool5 = new PoolingLayer("pool5", "relu5_3", "pool5", pool_pad, pool_kernelSize, pool_stride);
	layers_.push_back(pool5);

	// fc6
	FullyConnectedLayer* fc6 = new FullyConnectedLayer("fc6", "pool5", "fc6", 4096, fc_transpose);
	layers_.push_back(fc6);

	// fc7
	FullyConnectedLayer* fc7 = new FullyConnectedLayer("fc7", "fc6", "fc7", 4096, fc_transpose);
	layers_.push_back(fc7);

	// fc8
	FullyConnectedLayer* fc8 = new FullyConnectedLayer("fc8", "fc7", "fc8", 1000, fc_transpose);
	layers_.push_back(fc8);
}

void Net::SetBlobs()
{
	Logger::GetLogger()->LogMessage("\nNet::SetBlobs(): Setting up inter-layer blobs");

	blobs_.push_back(input_blob_); // start off blobs_
	Logger::GetLogger()->LogMessage("\nNet::SetBlobs(): Blob %s set up", input_blob_->name().c_str());

	Blob<float>* tempBlob;
	for(size_t layerIndex = 0; layerIndex < layers_.size()-1; layerIndex++)
	{
		if(layers_[layerIndex]->top() == layers_[layerIndex+1]->bottom())
		{
			tempBlob = new Blob<float>(layers_[layerIndex]->top());
			blobs_.push_back(tempBlob);
		}
		else
		{
			Logger::GetLogger()->LogError(
					"Net::Init",
					"Layer %i top blob name %s doesn't match layer %i bottom blob name %s"
					, layerIndex, layers_[layerIndex]->top().c_str(), layerIndex+1, layers_[layerIndex+1]->top().c_str()
			);
			return;
		}
		Logger::GetLogger()->LogMessage("\nNet::SetBlobs(): Blob %s set up", blobs_.back()->name().c_str());
	}

	output_blob_ = new Blob<float>(layers_.back()->top());
	blobs_.push_back(output_blob_);
	Logger::GetLogger()->LogMessage("\nNet::SetBlobs(): Blob %s set up", blobs_.back()->name().c_str());
}

void Net::Reshape()
{
	Logger::GetLogger()->LogMessage("\nNet::Reshape: Setting blob sizes");

	// propogate blob sizes resulting from input dimensions throught the network
	for(size_t layerIndex = 0; layerIndex < layers_.size(); layerIndex++)
	{
		layers_[layerIndex]->Reshape(blobs_[layerIndex], blobs_[layerIndex+1]);
	}
}

void Net::PrintLayerOutputShapes()
{
	std::cout << "Printing Layer Output Shapes:" << std::endl;
	std::cout << "[width*height*depth*num]\tlayer name\n" << std::endl;

	for(size_t layerIndex = 0; layerIndex < layers_.size(); layerIndex++)
	{
		blobs_[layerIndex]->PrintShape();
	}
}

void Net::Forward()
{
	Logger::GetLogger()->LogMessage("\nNet::Forward: Performing forward network computation");

	for(size_t layerIndex = 0; layerIndex < layers_.size(); layerIndex++)
	{
		Logger::GetLogger()->LogMessage("\nNet::Forward: Performing %s layer computation", layers_[layerIndex]->name().c_str());
		std::cout << "Performing " << layers_[layerIndex]->name() << " layer computation" << std::endl;
		layers_[layerIndex]->Forward(blobs_[layerIndex], blobs_[layerIndex+1]);
	}
}

void NetTest()
{
	// input dimensions
	int inputNum = 1, inputDepth = 3, inputHeight = 224, inputWidth = 224;

	Net net1("testNet", inputNum, inputDepth, inputHeight, inputWidth);
	net1.Init();

	FillConstant(net1.input_blob(), 1);

	net1.Forward();

	const float* outputData = net1.output_blob()->getConstData();
	for(int outputIndex = 0; outputIndex < net1.output_blob()->count(); outputIndex++)
	{
		std::cout << outputData[outputIndex] << std::endl;
	}
}

}
