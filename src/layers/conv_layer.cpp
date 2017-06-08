#include "conv_layer.hpp"

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
	num_output_ = num_output; // number of filters

	num_input_ = 0;
	output_size_ = 0;
	channels_ = 0;
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
			pad_, kernel_size_, stride_, num_output_
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

	// initialise and zero fill weights
	weights_.Reshape(num_output_, bottom->channels(), kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);

	kernel_volume_ = weights_.count(1);
}

void ConvolutionLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	num_input_ = bottom->num();
	channels_ = bottom->channels();
	input_size_ = bottom->height();

	float convolvedSize = ((float)(input_size_ + 2 * pad_ - kernel_size_) / stride_) + 1;
	if(convolvedSize != ceil(convolvedSize))
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"convedSize %f is non-integer",
				convolvedSize
		);
		return;
	}
	if((convolvedSize-1) * stride_ > input_size_ + pad_)
	{
		Logger::GetLogger()->LogError(
				"ConvolutionLayer::Reshape",
				"conv will not fit into output - (%i-1)*%i > %i+%i",
				convolvedSize, stride_, input_size_, pad_
		);
		return;
	}

	output_size_ = convolvedSize;
	Logger::GetLogger()->LogMessage("\tConvolutionLayer::Reshape: Convolved size = %i", output_size_);

	top->Reshape(num_input_, channels_, output_size_, output_size_);
	output_spatial_volume_ = top->count(2);

	col_buffer_.Reshape(1, kernel_volume_, output_size_, output_size_);
}

void ConvolutionLayer::ConvertBlobToInputColumns(const float* data_im, float* data_col)
{
	const int channelVolume = input_size_ * input_size_;

	for (int channel = channels_; channel--; data_im += channelVolume)
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

void ConvolutionLayer::Forward(const Blob<float>* bottom, Blob<float>* top)
{
	Logger::GetLogger()->LogMessage("\t%s layer performing forward computation", name_.c_str());

	const float* weight = weights_.getConstData();
	const float* bottomData = bottom->getConstData();
	float* topData = top->getMutableData();

	int bottomVolume = bottom->count(CHANNEL_AXIS);
	int topVolume = top->count(CHANNEL_AXIS);

	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
		conv_gemm_cpu(bottomData + numIndex * bottomVolume, weight, topData + numIndex * topVolume);
	}
}

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
			num_output_ , output_spatial_volume_, kernel_volume_, // m, n, k
			1., weights, col_buff, // alpha, A, B
			0., output // beta, C
	);
}

void ConvolutionLayer::conv_cpu(const Blob<float>* inputBlob, Blob<float>* outputBlob)
{
	const float* weights = weights_.getConstData();
	const float* input = inputBlob->getConstData();
	float* output = outputBlob->getMutableData();

	int paddedRow;
	int paddedCol;

	int num_input = num_input_;

	int Tr = 1;
	int Tc = 1;
	int Tm = 1;
	int Tn = 1;

	for(int row = 0; row < output_size_; row++)
	{
		for(int col = 0; col < output_size_; col++)
		{
			for(int to = 0; to < num_output_; to++)
			{
				for(int ti = 0; ti < num_input; ti++)
				{
					// load stuff
					int trrLim = std::min(row + Tr, output_size_);
					int tccLim = std::min(col + Tc, output_size_);
					int tooLim = std::min(to + Tm, num_output_);
					int tiiLim = std::min(ti + Tn, num_input);

					for(int trr = row; trr < trrLim; trr++)
					{
						for(int tcc = col; tcc < tccLim; tcc++)
						{
							for(int too = to; too < tooLim; too++)
							{
								for(int tii = ti; tii < tiiLim; tii++)
								{
									for(int i = 0; i < kernel_size_; i++)
									{
										paddedRow = stride_ * trr + i - pad_;

										for(int j = 0; j < kernel_size_; j++)
										{
											paddedCol = stride_ * tcc + j - pad_;

											if(paddedCol < 0 || paddedCol >= input_size_ || paddedRow < 0 || paddedRow >= input_size_)
											{
//												output[(too * trrLim + trr) * tccLim + tcc] += 0;
											}
											else
											{
												output[(too * output_size_ + trr) * output_size_ + tcc] +=
//												output[too][trr][tcc] +=
													weights[((too * output_size_ + tii) * kernel_size_ + i) * kernel_size_ + j] *
//													weights[too][tii][i][j] *
													input[(tii * input_size_ + paddedRow) * input_size_ + paddedCol];
//													input[tii][stride_ * trr + i][stride_ * tcc + j];
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


bool ConvTest()
{
	Logger::GetLogger()->LogMessage("Convolution Layer Test:");

	int num = 1, channels = 1, height = 7, width = 7;
	int count = num * channels * height * width;
	int stride = 2;
	int pad = 1; //0;
	int kernelSize = 3;
	int numOutputChannels = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	Blob<float> bottomBlob(num, channels, height, width);
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

	int num = 1, channels = 1, height = 7, width = 7;
	int count = num * channels * height * width;
	int stride = 2;
	int pad = 1; //0;
	int kernelSize = 3;
	int numOutputChannels = 1;

	std::cout << "pad = " << pad << ", stride =" << stride << ", kernel size = " << kernelSize << std::endl;
	std::cout << std::endl;

	ConvolutionLayer conv1("conv_test_im2col", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	ConvolutionLayer conv2("conv_test_conv", "test_in", "test_out", pad, kernelSize, stride, numOutputChannels); // initialise relu layer
	Blob<float> bottomBlob(num, channels, height, width);
	Blob<float> colBlob, convBlob;

	conv1.SetUp(&bottomBlob, &colBlob);
	conv2.SetUp(&bottomBlob, &convBlob);

	// set input data
	float *dataIn = bottomBlob.getMutableData();
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex/height + 1; // dataIndex/height will nautrally be the floor of this as these are both ints
	}

	// set weights
	FillConstant(&conv1.weights_, 1);
	FillConstant(&conv2.weights_, 1);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	std::cout << "Weights Slice" << std::endl;
	conv1.weights_.PrintSlice(0, 0);

	conv1.Forward(&bottomBlob, &colBlob); // perform forward computation
	conv2.conv_cpu(&bottomBlob, &convBlob); // perform forward computation

	// print results
	std::cout << "Top Data Slice" << std::endl;
	colBlob.PrintSlice(0, 0);
	convBlob.PrintSlice(0, 0);

	// check results
	bool testPassed = true;
	const float* colData = colBlob.getConstData();
	const float* convData = convBlob.getConstData();

	for(int dataIndex = 0; dataIndex < colBlob.count(); dataIndex++)
	{
		bool testPassed_temp = (colData[dataIndex] == convData[dataIndex]);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"ConvCompare",
					"Conv output %.1f incorrect at index = %i, expected %.1f",
					convData[dataIndex], dataIndex, colData[dataIndex]
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


}
