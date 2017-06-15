#include "conv_layer.hpp"

#include <sds_lib.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "../logger.hpp"
#include "../util/filler.hpp"
#include "../util/maths_functions.hpp"
#include "../util/timer.hpp"
#include "../hw_functions.h"

ConvolutionLayer::ConvolutionLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride, int num_output)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	kernel_size_ = kernelSize;
	stride_ = stride;
	pad_ = pad;
	output_depth_ = num_output; // number of filters

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
			pad_, kernel_size_, stride_, output_depth_
	);
}

void ConvolutionLayer::LayerSetUp(const Blob<int>* bottom, const Blob<int>* top)
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

void ConvolutionLayer::Reshape(const Blob<int>* bottom, Blob<int>* top)
{
	num_input_ = bottom->num();
	input_depth_ = bottom->depth();
	input_size_ = bottom->height();

	int inputSize = input_size_;

	int convolvedSize = ((int)(input_size_ + 2 * pad_ - kernel_size_) / stride_) + 1;
	if(convolvedSize != ceil((float)(input_size_ + 2 * pad_ - kernel_size_) / stride_) + 1)
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

	top->Reshape(num_input_, output_depth_, output_size_, output_size_);
	output_spatial_volume_ = top->count(2);

	// initialise and zero fill weights
	weights_.Reshape(output_depth_, bottom->depth(), kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);

	col_buffer_.Reshape(1, kernel_volume_, output_size_, output_size_);
}

//Deprecated
void ConvolutionLayer::ConvertBlobToInputColumns(const int* data_im, int* data_col)
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
void ConvolutionLayer::Forward_gemm(const Blob<int>* bottom, Blob<int>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	const int* weight = weights_.getConstData();
	const int* bottomData = bottom->getConstData();
	int* topData = top->getMutableData();

	int bottomVolume = bottom->count(DEPTH_AXIS);
	int topVolume = top->count(DEPTH_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		conv_gemm_cpu(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

// Deprecated
void ConvolutionLayer::conv_gemm_cpu(const int* input, const int* weights, int* output, bool skip_im2col)
{
	const int* col_buff = input;
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
			output_depth_ , output_spatial_volume_, kernel_volume_, // m, n, k
			1., weights, col_buff, // alpha, A, B
			0., output // beta, C
	);
}

void ConvolutionLayer::Forward(const Blob<int>* bottom, Blob<int>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	FillConstant(top, 0);

	const int* weight = weights_.getConstData();
	const int* bottomData = bottom->getConstData();
	int* topData = top->getMutableData();

	int bottomVolume = bottom->count(DEPTH_AXIS);
	int topVolume = top->count(DEPTH_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		Convolution(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

void ConvolutionLayer::Convolution(const int* input, const int* weights, int* output)
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
			for(int outDepth = 0; outDepth < output_depth_; outDepth += outDepthTileSize)
			{
				for(int inDepth = 0; inDepth < input_depth_; inDepth += inDepthTileSize)
				{
					// load stuff
					int outRowTileEnd = std::min(outRow + rowTileSize, output_size_);
					int outColTileEnd = std::min(outCol + colTileSize, output_size_);
					int outDepthTileEnd = std::min(outDepth + outDepthTileSize, output_depth_);
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

void ConvolutionLayer::Forward_hw(const Blob<int>* bottom, Blob<int>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	FillConstant(top, 0);

	const int* weight = weights_.getConstData();
	const int* bottomData = bottom->getConstData();
	int* topData = top->getMutableData();

	int bottomVolume = bottom->count(DEPTH_AXIS);
	int topVolume = top->count(DEPTH_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		Convolution_hw(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

void ConvolutionLayer::Convolution_hw(const int* input, const int* weights, int* output)
{
	// Tiling values
	int outRowTileSize = OUT_ROW_TILE_SIZE_3X3;
	int outColTileSize = OUT_COL_TILE_SIZE_3X3;
	int outDepthTileSize = OUT_DEPTH_TILE_SIZE_3X3;
	int inDepthTileSize = IN_DEPTH_TILE_SIZE_3X3;

	int inRowTileSize = outRowTileSize + 2; // would need to change 2 for different stride/kernel_size
	int inColTileSize = outColTileSize + 2; // would need to change 2 for different stride/kernel_size

	for(int outRowTileStart = 0; outRowTileStart < output_size_; outRowTileStart += outRowTileSize)
	{
		for(int outColTileStart = 0; outColTileStart < output_size_; outColTileStart += outColTileSize)
		{
			for(int outDepthTileStart = 0; outDepthTileStart < output_depth_; outDepthTileStart += outDepthTileSize)
			{
				for(int inDepthTileStart = 0; inDepthTileStart < input_depth_; inDepthTileStart += inDepthTileSize)
				{
					// load stuff
					int outRowTileEnd = std::min(outRowTileStart + outRowTileSize, output_size_);
					int outColTileEnd = std::min(outColTileStart + outColTileSize, output_size_);
					int outDepthTileEnd = std::min(outDepthTileStart + outDepthTileSize, output_depth_);
					int inDepthTileEnd = std::min(inDepthTileStart + inDepthTileSize, input_depth_);

					int outputTileLength = outDepthTileSize * outRowTileSize * outColTileSize;
					int weightsTileLength = outDepthTileSize * inDepthTileSize * kernel_size_ * kernel_size_;
					int inputTileLength = inDepthTileSize * inRowTileSize * inColTileSize;

					int* outputTile = (int*)sds_alloc(outputTileLength * sizeof(int));
					int* weightsTile = (int*)sds_alloc(weightsTileLength * sizeof(int));
					int* inputTile = (int*)sds_alloc(inputTileLength * sizeof(int));

					if (!inputTile || !weightsTile || !outputTile)
					{
						if(inputTile)
						{
							free(inputTile);
						}
						if(weightsTile)
						{
							free(weightsTile);
						}
						if(outputTile)
						{
							free(outputTile);
						}
						return;
					}

//					for(int outRowIndex = outRowTileStart; outRowIndex < outRowTileEnd; outRowIndex++)
//					{
//						for(int outColIndex = outColTileStart; outColIndex < outColTileEnd; outColIndex++)
//						{
//							for(int outDepthIndex = outDepthTileStart; outDepthIndex < outDepthTileEnd; outDepthIndex++)
//							{
//								outputTile[((outDepthIndex-outDepthTileStart) * outRowTileEnd + (outRowIndex-outRowTileStart)) * outColTileEnd + (outColIndex-outColTileStart)]
//										   = output[(outDepthIndex * output_size_ + outRowIndex) * output_size_ + outColIndex];
//
//							}
//						}
//					}

					std::cout << inputTileLength << std::endl;

					FillConstant(weightsTile, weightsTileLength, 1);
					FillConstant(inputTile, inputTileLength, 1);

					conv_hw_conv3x3(input_size_, output_size_, input_depth_,
							outRowTileStart, outRowTileEnd,
							outColTileStart, outColTileEnd,
							outDepthTileStart, outDepthTileEnd,
							inDepthTileStart, inDepthTileEnd,
							inputTile, weightsTile, outputTile
					);

					for(int outRowIndex = outRowTileStart; outRowIndex < outRowTileEnd; outRowIndex++)
					{
						for(int outColIndex = outColTileStart; outColIndex < outColTileEnd; outColIndex++)
						{
							for(int outDepthIndex = outDepthTileStart; outDepthIndex < outDepthTileEnd; outDepthIndex++)
							{
								output[(outDepthIndex * output_size_ + outRowIndex) * output_size_ + outColIndex] +=
										outputTile[((outDepthIndex-outDepthTileStart) * outRowTileSize + (outRowIndex-outRowTileStart)) * outColTileSize + (outColIndex-outColTileStart)];
//								std::cout << (outDepthIndex * output_size_ + outRowIndex) * output_size_ + outColIndex << " " << outputTile[((outDepthIndex-outDepthTileStart) * outRowTileEnd + (outRowIndex-outRowTileStart)) * outColTileEnd + (outColIndex-outColTileStart)] << std::endl;
							}
						}
					}

					sds_free(outputTile);
					sds_free(weightsTile);
					sds_free(inputTile);
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
	int outputDepth = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test", "test_in", "test_out", pad, kernelSize, stride, outputDepth); // initialise relu layer
	Blob<int> bottomBlob(num, depth, height, width);
	Blob<int> topBlob;

	conv1.SetUp(&bottomBlob, &topBlob);

	// set input data
	int *dataIn = bottomBlob.getMutableData();
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
	const int* topData = topBlob.getConstData();
	int trueResults[] = {6, 9, 9, 6, 18, 27, 27, 18, 30, 45, 45, 30, 26, 39, 39, 26};

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

bool ConvHardwareTest()
{
	Logger::GetLogger()->LogMessage("Convolution Layer Test:");

	int num = 1, depth = 1, height = 7, width = 7;
	int count = num * depth * height * width;
	int stride = 2;
	int pad = 1;
	int kernelSize = 3;
	int outputDepth = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test", "test_in", "test_out", pad, kernelSize, stride, outputDepth); // initialise relu layer
	Blob<int> bottomBlob(num, depth, height, width);
	Blob<int> topBlob;

	conv1.SetUp(&bottomBlob, &topBlob);

	// set input data
	int *dataIn = bottomBlob.getMutableData();
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
	const int* topData = topBlob.getConstData();
	int trueResults[] = {6, 9, 9, 6, 18, 27, 27, 18, 30, 45, 45, 30, 26, 39, 39, 26};

	int outDepth = topBlob.depth();
	int outHeight = topBlob.height();
	int outWidth = topBlob.width();

	for(int depthIndex = 0; depthIndex < outDepth; depthIndex++)
	{
		for(int heightIndex = 0; heightIndex < outHeight; heightIndex++)
		{
			for(int widthIndex = 0; widthIndex < outWidth; widthIndex++)
			{
				int arryIndex = (depthIndex * outHeight + heightIndex) * outWidth + widthIndex;
				bool testPassed_temp = (topData[arryIndex] == trueResults[arryIndex]);
				if(!testPassed_temp)
				{
					Logger::GetLogger()->LogError(
							"ConvTest",
							"Output %.1f incorrect at (%i, %i, %i), expected %i",
							topData[arryIndex], depthIndex, heightIndex, widthIndex, trueResults[arryIndex]
					);
				}
				testPassed &= testPassed_temp; // AND test into overall test result
			}
		}
	}
//
//	for(int dataIndex = 0; dataIndex < topBlob.count(); dataIndex++)
//	{
//		bool testPassed_temp = (topData[dataIndex] == trueResults[dataIndex]);
//		if(!testPassed_temp)
//		{
//			Logger::GetLogger()->LogError(
//					"ConvTest",
//					"Output %.1f incorrect at index = %i, expected %i",
//					topData[dataIndex], dataIndex, trueResults[dataIndex]
//			);
//		}
//		testPassed &= testPassed_temp; // AND test into overall test result
//	}

	std::string resultString = "\tConvolution Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}

void ConvSpeed()
{
	Logger::GetLogger()->LogMessage("Convolution Speed Test:");
	Timer timer;

	int num = 1, depth = 1, inputSize = 3;
	int stride = 1;
	int pad = 1;
	int kernelSize = 3;
	int outputDepth = 1;

	int numTests = 128;

	ConvolutionLayer conv1("conv_test", "test_in", "test_out", pad, kernelSize, stride, outputDepth); // initialise relu layer
	Blob<int> bottomBlob(num, depth, inputSize, inputSize);
	Blob<int> topBlob;

	conv1.SetUp(&bottomBlob, &topBlob);

	// set input data
	FillConstant(&bottomBlob, 1);

	// set weights
	FillConstant(&conv1.weights_, 1);

	for(int testCounter = 0; testCounter < numTests; testCounter++)
	{
		timer.start();
		conv1.Forward(&bottomBlob, &topBlob); // perform forward computation
		timer.stop();
	}

	std::cout << "Average CPU cycles = " << timer.getAverageCpuTime() << std::endl;

}


bool ConvCompare()
{
	Logger::GetLogger()->LogMessage("Convolution Implementation Test:");

	int num = 1, depth = 3, height = 14, width = 14;
	int stride = 1;
	int pad = 1;
	int kernelSize = 3;
	int numOutputChannels = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test_sw", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	ConvolutionLayer conv2("conv_test_hw", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	Blob<int> bottomBlob(num, depth, height, width);
	Blob<int> swBlob, hwBlob;

	conv1.SetUp(&bottomBlob, &swBlob);
	conv2.SetUp(&bottomBlob, &hwBlob);

	// set input data
	FillConstant(&bottomBlob, 1);

	// set weights
	FillConstant(&conv1.weights_, 1);
	conv2.weights_.CopyFrom(&conv1.weights_, true);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	std::cout << "Weights Slice" << std::endl;
	conv1.weights_.PrintSlice(0, 0);

	std::cout << "Starting sw test" << std::endl;
	conv1.Forward(&bottomBlob, &swBlob); // perform forward computation
	std::cout << "Starting hw test" << std::endl;
	conv2.Forward_hw(&bottomBlob, &hwBlob); // perform forward computation

	// print results
	std::cout << "Top Data Slices" << std::endl;
	std::cout << "sw implementation" << std::endl;
//	swBlob.PrintSlice(0, 0);
	std::cout << "hw implementation" << std::endl;
//	hwBlob.PrintSlice(0, 0);
//	hwBlob.PrintSlice(0, 1);
//	hwBlob.PrintSlice(0, 2);

	// check results
	bool testPassed = true;
	const int* swData = swBlob.getConstData();
	const int* hwData = hwBlob.getConstData();

	int outDepthLim = hwBlob.depth();
	int outHeightLim = hwBlob.height();
	int outWidthLim = hwBlob.width();

	for(int depthIndex = 0; depthIndex < outDepthLim; depthIndex++)
	{
		for(int heightIndex = 0; heightIndex < outHeightLim; heightIndex++)
		{
			for(int widthIndex = 0; widthIndex < outWidthLim; widthIndex++)
			{
				int arryIndex = (depthIndex * outHeightLim + heightIndex) * outWidthLim + widthIndex;
				bool testPassed_temp = (hwData[arryIndex] == swData[arryIndex]);
				if(!testPassed_temp)
				{
					Logger::GetLogger()->LogError(
							"ConvTest",
							"Output %.1f incorrect at (%i, %i, %i), expected %i",
							hwData[arryIndex], depthIndex, heightIndex, widthIndex, swData[arryIndex]
					);
				}
				testPassed &= testPassed_temp; // AND test into overall test result
			}
		}
	}

	std::string resultString = "\tConvolution Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}
