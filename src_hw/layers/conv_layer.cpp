#include "conv_layer.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "../logger.hpp"
#include "../util/filler.hpp"
#include "../util/maths_functions.hpp"


namespace spaceBrain
{

ConvolutionLayer::ConvolutionLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride, int num_output)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	kernel_size_ = kernelSize;
	stride_ = stride;
	pad_ = pad;
	num_kernels = num_output; // number of filters

	num_input_ = 0;
	output_size_ = 0;
	input_depth_ = 0;
	input_size_ = 0;
	output_spatial_volume_ = 0;
	kernel_volume_ = 0;

	is_1x1_ = false;

	Logger::GetLogger()->LogMessage(
			"\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tpad = %i, kernelSize = %i, stride = %i, num kernels = %i",
			pad_, kernel_size_, stride_, num_kernels
	);
}

void ConvolutionLayer::LayerSetUp(const Blob<float>* bottom, const Blob<float>* top)
{
	// check that each parameter is valid
	bool paramTestsPassed = true;
	if(kernel_size_ < 2)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Kernel size %i < 2",
				kernel_size_
		);
	}
	if(pad_ < 0)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Padding size %i < 0",
				pad_
		);
	}
	if(stride_ < 1)
	{
		paramTestsPassed = true;
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"stride %i < 1",
				stride_
		);
	}

	// check that bottom blob is square
	bool squareCheckPassed = bottom->height() == bottom->width();
	if(!squareCheckPassed)
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::LayerSetUp",
				"Bottom blob shape is not square"
		);
	}

	if(!squareCheckPassed || !paramTestsPassed)
	{
		std::cerr << "ConvolutionLayer::LayerSetUp Error - check log";
	}

	// 1x1 is a special case where im2col is not needed
	is_1x1_ = (kernel_size_ == 1 && stride_ == 1 && pad_ == 0);

	kernel_volume_ = weights_.count(1);
}

void ConvolutionLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	num_input_ = bottom->num();
	input_depth_ = bottom->depth();
	input_size_ = bottom->height();

	int inputSize = input_size_;

	float convolvedSize = ((float)(inputSize + 2 * pad_ - kernel_size_) / stride_) + 1;
	if(convolvedSize != ceil(convolvedSize))
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"convedSize %f is non-integer",
				convolvedSize
		);
		return;
	}
	if((convolvedSize-1) * stride_ > inputSize + pad_)
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"conv will not fit into output - (%i-1)*%i > %i+%i",
				convolvedSize, stride_, inputSize, pad_
		);
		return;
	}

	output_size_ = convolvedSize;
	Logger::GetLogger()->LogMessage("\tConvolutionLayer::Reshape: Convolved size = %i", output_size_);

	top->Reshape(num_input_, num_kernels, output_size_, output_size_);
	output_spatial_volume_ = top->count(2);

	// initialise and zero fill weights
	weights_.Reshape(num_kernels, bottom->depth(), kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);

	col_buffer_.Reshape(1, kernel_volume_, output_size_, output_size_);
}

//Deprecated
void ConvolutionLayer::ConvertBlobToInputColumns(const float* data_im, float* data_col)
{
	const int channelVolume = input_size_ * input_size_;

	for (int channel = input_depth_; channel--; data_im += channelVolume)
	{
		for (int kernel_row = 0; kernel_row < kernel_size_; kernel_row++)
		{
			for (int kernel_col = 0; kernel_col < kernel_size_; kernel_col++)
			{
				int input_row = -pad_ + kernel_row;
				for (int output_rows = output_size_; output_rows; output_rows--)
				{
					if (!(input_row >= 0 && input_row < input_size_))
					{
						// region is in padding
						for (int output_cols = output_size_; output_cols; output_cols--)
						{
							*(data_col++) = 0;
						}
					}
					else
					{
						int input_col = -pad_ + kernel_col;
						for (int output_col = output_size_; output_col; output_col--)
						{
							if (input_col >= 0 && input_col < input_size_)
							{
								*(data_col++) = data_im[input_row * input_size_ + input_col]; // assign data to *data_col
							}
							else
							{
								*(data_col++) = 0; // data point is in padding
							}
							input_col += stride_;
						}
					}
					input_row += stride_;
				} /* output rows */
			} /* kernel column */
		} /* kernel row */
	} /* channels */
}

// Deprecated
void ConvolutionLayer::Forward_gemm(const Blob<float>* bottom, Blob<float>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	const float* weight = weights_.getConstData();
	const float* bottomData = bottom->getConstData();
	float* topData = top->getMutableData();

	int bottomVolume = bottom->count(DEPTH_AXIS);
	int topVolume = top->count(DEPTH_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		conv_gemm_cpu(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

// Deprecated
void ConvolutionLayer::conv_gemm_cpu(const float* input, const float* weights, float* output, bool skip_im2col)
{
	const float* col_buff = input;
	// perform im2col or skip
	if (!is_1x1_)
	{
		if (!skip_im2col)
		{
			ConvertBlobToInputColumns(input, col_buffer_.getMutableData());

		}
		col_buff = col_buffer_.getConstData();
	}
	// perform gemm
	gemm_cpu(
			false, false, 	// transposes
			num_kernels , output_spatial_volume_, kernel_volume_, // m, n, k
			1., weights, col_buff, // alpha, A, B
			0., output // beta, C
	);
}

void ConvolutionLayer::Forward(const Blob<float>* bottom, Blob<float>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	FillConstant(top, 0);

	const float* weight = weights_.getConstData();
	const float* bottomData = bottom->getConstData();
	float* topData = top->getMutableData();

	int bottomVolume = bottom->count(DEPTH_AXIS);
	int topVolume = top->count(DEPTH_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		Convolution(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

void ConvolutionLayer::Convolution(const float* input, const float* weights, float* output)
{
	// Tiling values
	int rowTileSize = 4;
	int colTileSize = 4;
	int outDepthTileSize = 4;
	int inDepthTileSize = 4;

	for(int outRow = 0; outRow < output_size_; outRow += rowTileSize)
	{
		for(int outCol = 0; outCol < output_size_; outCol += colTileSize)
		{
			for(int outDepth = 0; outDepth < num_kernels; outDepth += outDepthTileSize)
			{
				for(int inDepth = 0; inDepth < input_depth_; inDepth += inDepthTileSize)
				{
					// load stuff
					int outRowTileEnd = std::min(outRow + rowTileSize, output_size_);
					int outColTileEnd = std::min(outCol + colTileSize, output_size_);
					int outDepthTileEnd = std::min(outDepth + outDepthTileSize, num_kernels);
					int inDepthTileEnd = std::min(inDepth + inDepthTileSize, input_depth_);

					conv_cpu(stride_, pad_,
							input_size_, kernel_size_, output_size_, input_depth_,
							outRow, outRowTileEnd,
							outCol, outColTileEnd,
							outDepth, outDepthTileEnd,
							inDepth, inDepthTileEnd,
							input, weights, output
					);
				}
			}
		}
	}
}


bool ConvTest()
{
	Logger::GetLogger()->LogMessage("Convolution Layer Test:");

	int num = 1, depth = 1, height = 7, width = 7;
	int count = num * depth * height * width;
	int stride = 2;
	int pad = 1;
	int kernelSize = 3;
	int numOutputChannels = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	Blob<float> bottomBlob(num, depth, height, width);
	Blob<float> topBlob;

	conv1.SetUp(&bottomBlob, &topBlob);

	// set input data
	float *dataIn = bottomBlob.getMutableData();
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex/height + 1; // dataIndex/height will nautrally be the floor of this as these are both ints
	}

	// set weights
	FillConstant(&conv1.weights_, 1);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	std::cout << "Weights Slice" << std::endl;
	conv1.weights_.PrintSlice(0, 0);

	conv1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// print results
	std::cout << "Top Data Slice" << std::endl;
	topBlob.PrintSlice(0, 0);

	// check results
	bool testPassed = true;
	const float* topData = topBlob.getConstData();
	float trueResults[] = {6, 9, 9, 6, 18, 27, 27, 18, 30, 45, 45, 30, 26, 39, 39, 26};

	for(int dataIndex = 0; dataIndex < topBlob.count(); dataIndex++)
	{
		bool testPassed_temp = (topData[dataIndex] == trueResults[dataIndex]);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"ConvTest",
					"Output %.1f incorrect at index = %i, expected %i",
					topData[dataIndex], dataIndex, trueResults[dataIndex]
			);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tConvolution Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}

bool ConvCompare()
{
	Logger::GetLogger()->LogMessage("Convolution Implementation Test:");

	int num = 1, depth = 1, height = 14, width = 14;
	int stride = 1;
	int pad = 1;
	int kernelSize = 3;
	int numOutputChannels = 3;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test_im2col", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	ConvolutionLayer conv2("conv_test_conv", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	Blob<float> bottomBlob(num, depth, height, width);
	Blob<float> im2colBlob, convBlob;

	conv1.SetUp(&bottomBlob, &im2colBlob);
	conv2.SetUp(&bottomBlob, &convBlob);

	// set input data
	FillConstant(&bottomBlob, 1);

	// set weights
	FillConstant(&conv1.weights_, 1);
	conv2.weights_.CopyFrom(&conv1.weights_, true);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	std::cout << "Weights Slice" << std::endl;
	conv1.weights_.PrintSlice(0, 0);

	std::cout << "Starting im2col gemm test" << std::endl;
	conv1.Forward_gemm(&bottomBlob, &im2colBlob); // perform forward computation
	std::cout << "Starting zhang conv test" << std::endl;
	conv2.Forward(&bottomBlob, &convBlob); // perform forward computation

	// print results
	std::cout << "Top Data Slices" << std::endl;
	std::cout << "im2col gemm implementation" << std::endl;
	im2colBlob.PrintSlice(0, 0);
	std::cout << "FPGA friendy implementation" << std::endl;
	convBlob.PrintSlice(0, 0);

	// check results
	bool testPassed = true;
	const float* colData = im2colBlob.getConstData();
	const float* convData = convBlob.getConstData();

	for(int dataIndex = 0; dataIndex < im2colBlob.count(); dataIndex++)
	{
		bool testPassed_temp = (colData[dataIndex] == convData[dataIndex]);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"ConvCompare",
					"Conv output %.1f incorrect at index = %i, expected %.1f",
					convData[dataIndex], dataIndex, colData[dataIndex]
			);
//			std::cout << convData[dataIndex] << " "<< dataIndex << " " << colData[dataIndex] << std::endl;
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

//	std::cout << convData

	std::string resultString = "\tConvolution Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}

}
