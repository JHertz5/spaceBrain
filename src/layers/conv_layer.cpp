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
	num_output_channels_ = num_output; // number of filters
	num_input_channels_ = 1; // TODO check what's going on here

	output_size_ = 0;
	channels_ = 0;
	input_size_ = 0;

	is_1x1_ = false;

	Logger::GetLogger()->LogMessage(
			"\tConvolution layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tpad = %i, kernelSize = %i, stride = %i, num outputs = %i, num inputs = %i",
			pad_, kernel_size_, stride_, num_output_channels_, num_input_channels_
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
	weights_.Reshape(num_output_channels_, num_input_channels_, kernel_size_, kernel_size_);
	FillConstant(&weights_, 0);
}

void ConvolutionLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	channels_ = bottom->channels();
	input_size_ = bottom->height();
	input_size_ = bottom->width();

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

	top->Reshape(bottom->num(), channels_, output_size_, output_size_);

	// TODO rename to column buffer
	int num_columns = channels_ * kernel_size_ * kernel_size_;
	col_buffer_.Reshape(1, num_columns, output_size_, output_size_);
}

void ConvolutionLayer::im2col(const float* data_im, float* data_col)
{
//	float* data_col = col_buffer_.getMutableData();
	const int channel_size = input_size_ * input_size_;

	for (int channel = channels_; channel--; data_im += channel_size)
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

	// TODO check bottom->num()
	for(int numIndex = 0; numIndex < bottom->num(); numIndex++)
	{
//		Forward_gemm_cpu()
	}



	FillConstant(top, 0);

	Logger::GetLogger()->LogMessage("ConvolutionLayer::Forward() NOT YET IMPLEMENTED");
}

void ConvolutionLayer::forward_cpu_gemm(const float* input, const float* weights, float* output, bool skip_im2col)
{
	const float* col_buff = input;
	// perform im2col or skip
	if (!is_1x1_)
	{
		if (!skip_im2col)
		{
			im2col(input, col_buffer_.getMutableData());

		}
		col_buff = col_buffer_.getConstData();
	}
	// perform gemm
//	gemm_cpu(
//			CblasNoTrans, CblasNoTrans,
//			conv_out_channels_ / group_, conv_out_spatial_dim_, kernel_dim_,
//	        1., weights + weight_offset_ * g, col_buff + col_offset_ * g,
//	        0., output + output_offset_ * g);
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
	float *dataIn = new float[count];
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex;
	}
	bottomBlob.SetData(dataIn,count);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	conv1.im2col(bottomBlob.getConstData(), conv1.col_buffer_.getMutableData());
	//conv1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// print results
//	std::cout << "Top Data" << std::endl;
//	topBlob.PrintSlice();
	std::cout << "Column Buffer" << std::endl;

	// check results
	bool testPassed = true;
/*	for(int hIndex = 0; hIndex< topBlob.height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < topBlob.width(); wIndex++)
		{
			int bottomHIndex = (stride * hIndex) + topBlob.height() - 2 * pad - 1;
			int bottomWIndex = (stride * wIndex) + topBlob.width() - 2 * pad - 1;

			if(hIndex == topBlob.height()-1)
			{
				bottomHIndex--;
			}
			if(wIndex == topBlob.width()-1)
			{
				bottomWIndex--;
			}

			bool testPassed_temp = topBlob.getDataAt(0, 0, hIndex, wIndex) == bottomBlob.getDataAt(0, 0, bottomHIndex, bottomWIndex);
			if(!testPassed_temp)
			{
				Logger::GetLogger()->LogError(
						"ConvTest",
						"Conv output incorrect at index: %i,%i",
						hIndex, wIndex
				);
			}
			testPassed &= testPassed_temp; // AND test into overall test result
		}
	}
*/
	std::string resultString = "\tConvolution Layer Test ";
	resultString += "NOT YET IMPLEMENTED\n";//(testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}

}
