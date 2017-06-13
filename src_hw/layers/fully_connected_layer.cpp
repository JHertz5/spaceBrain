#include "fully_connected_layer.hpp"

#include <algorithm>
#include <iostream>

#include "../logger.hpp"
#include "../util/filler.hpp"
#include "../util/maths_functions.hpp"

// http://tech.gilt.com/deep/learning/2016/05/18/fully-connected-to-convolutional-conversion
// TODO change size_ to a bettter name (spatial length?)

FullyConnectedLayer::FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output, bool transpose)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	transpose_ = transpose;
	output_depth_ = num_output;

	input_num_ = 0;
	input_size_ = 0;
	input_depth_ = 0;
	output_size_ = 0;
	input_volume_ = 0;

	Logger::GetLogger()->LogMessage(
			"\tFully Connected layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tnum ouptputs = %i",
			output_depth_
	);
}

void FullyConnectedLayer::LayerSetUp(const Blob<float>* bottom, const Blob<float>* top)
{
	input_volume_ = bottom->count(DEPTH_AXIS);
	input_depth_ = bottom->depth();
	input_size_ = bottom->height();

	weights_.Reshape(output_depth_, input_depth_, input_size_, input_size_);
}

void FullyConnectedLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	input_num_ = bottom->num();

	top->Reshape(bottom->num(), output_depth_, 1 , 1);

	output_size_ = top->height();
}

void FullyConnectedLayer::Forward_gemm(const Blob<float>* bottom, Blob<float>* top)
{
	const float* bottom_data = bottom->getConstData();
	float* top_data = top->getMutableData();
	const float* weight = this->weights_.getConstData();

	// treat input and weights as if they were flattened in the depth, height, width dimensions
	gemm_cpu(
			false, transpose_, // transposes
			input_num_, output_depth_, input_volume_, // m, n, k
			1., bottom_data, weight, // alpha, A, B
			0., top_data // beta, C
	);
}

void FullyConnectedLayer::Forward(const Blob<float>* bottom, Blob<float>* top)
{
	const float* bottom_data = bottom->getConstData();
	float* top_data = top->getMutableData();
	const float* weight = this->weights_.getConstData();

	// note - if using Caffe weights, the weights vector will need to be transposed to acheive the same result with this implementation
	// gemm version treats spatial dimensions as the second dimension to iterate through
	// this version iterates through the spatial dimensions first
	Convolution(bottom_data, weight, top_data);
}


void FullyConnectedLayer::Convolution(const float* input, const float* weights, float* output)
{
	// Tiling values
	int rowTileSize = 4;
	int colTileSize = 4;
	int outDepthTileSize = 4;
	int inDepthTileSize = 4;

	int pad = 0;
	int stride = 1;

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

					// transpose refers to gemm implementation in order to match Caffe
					// in order to match gemm with tranpose = false, this must have transpose = true, and vice versa
					if(!transpose_)
					{
						conv_cpu_transB(stride, pad,
								input_size_, input_size_, output_size_, output_depth_, // input size and kernel size are equal
								outRow, outRowTileEnd,
								outCol, outColTileEnd,
								outDepth, outDepthTileEnd,
								inDepth, inDepthTileEnd,
								input, weights, output
						);
					}
					else
					{
						conv_cpu(stride, pad,
								input_size_, input_size_, output_size_, input_depth_, // input size and kernel size are equal
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
}

bool FullyConnectedTest()
{
	Logger::GetLogger()->LogMessage("Fully Connected Layer Test:");

	int num = 1, depth = 3, height = 7, width = 7;
	int num_output = 3;
	bool transpose = false;

	FullyConnectedLayer fc1("fc_test", "test_in", "test_out", num_output, transpose);
	Blob<float> bottomBlob(num, depth, height, width);
	Blob<float> topBlob;

	fc1.SetUp(&bottomBlob, &topBlob);

	// set input data
//	FillConstant(&bottomBlob, 1);
	float* inputData = bottomBlob.getMutableData();
	int inputVolume = bottomBlob.count(HEIGHT_AXIS);
	FillConstant(inputData, inputVolume, 1);
	FillConstant(inputData + inputVolume, inputVolume, 2);
	FillConstant(inputData + 2*inputVolume, inputVolume, 3);

	// set weights
//	FillConstant(&fc1.weights_, 1);
	float* weightsData = fc1.weights_.getMutableData();
	int weightsVolume = fc1.weights_.count(DEPTH_AXIS);
	FillConstant(weightsData, weightsVolume, 1);
	FillConstant(weightsData + weightsVolume, weightsVolume, 2);
	FillConstant(weightsData + 2*weightsVolume, weightsVolume, 3);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();
	std::cout << bottomBlob.depth() << "*" << bottomBlob.height() << "*" << bottomBlob.width() << "\n" << std::endl;

	std::cout << "Weights Slice" << std::endl;
	fc1.weights_.PrintSlice(0, 0);
	std::cout << fc1.weights_.num() << "*" << fc1.weights_.depth() << "*" << fc1.weights_.height() << "*" << fc1.weights_.width() << " ones\n" << std::endl;

	fc1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// print results
	std::cout << "Top Data" << std::endl;
	topBlob.PrintSlice(0,0);
	topBlob.PrintSlice(0,1);
	topBlob.PrintSlice(0,2);
	std::cout << topBlob.depth() << "*" << topBlob.height() << "*" << topBlob.width() << "\n" << std::endl;

	// check results
	bool testPassed = true;
	const float* topData = topBlob.getConstData();
	float trueResult = 686;

	for(int dataIndex = 0; dataIndex < topBlob.count(); dataIndex++)
	{
		bool testPassed_temp = (topData[dataIndex] == trueResult);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"FullyConnectedTest",
					"Output %.0f incorrect at index = %i, expected %i",
					topData[dataIndex], dataIndex, trueResult
			);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tFully Connected Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}


bool FullyConnectedCompare()
{
	Logger::GetLogger()->LogMessage("Fully Connected Implementation Test:");

	int num = 1, depth = 3, height = 7, width = 7;
	int num_output = 3;
	bool transpose = false;

	FullyConnectedLayer fc1("fc_test_gemm", "test_in", "test_out", num_output, transpose); // initialise relu layer
	FullyConnectedLayer fc2("fc_test_conv", "test_in", "test_out", num_output, transpose); // initialise relu layer
	Blob<float> bottomBlob(num, depth, height, width);
	Blob<float> gemmBlob, convBlob;

	fc1.SetUp(&bottomBlob, &gemmBlob);
	fc2.SetUp(&bottomBlob, &convBlob);

	// set input data
//	FillConstant(&bottomBlob, 1);
	float* inputData = bottomBlob.getMutableData();
	int input2dVolume = bottomBlob.count(HEIGHT_AXIS);
	FillConstant(inputData, input2dVolume, 1);
	FillConstant(inputData + input2dVolume, input2dVolume, 2);
	FillConstant(inputData + 2*input2dVolume, input2dVolume, 3);

	// set weights
//	FillConstant(&fc1.weights_, 1);
	float* weightsData = fc1.weights_.getMutableData();
	int weightsVolume = fc1.weights_.count(DEPTH_AXIS);
	FillConstant(weightsData, weightsVolume, 1);
	FillConstant(weightsData + weightsVolume, weightsVolume, 2);
	FillConstant(weightsData + 2*weightsVolume, weightsVolume, 3);
	fc2.weights_.CopyFrom(&fc1.weights_, true);

	std::cout << "Bottom Data" << std::endl;
	bottomBlob.PrintSlice();

	std::cout << "Weights Slice" << std::endl;
	fc1.weights_.PrintSlice(0, 0);
	fc2.weights_.PrintSlice(0, 0);

	std::cout << "Starting gemm test" << std::endl;
	fc1.Forward_gemm(&bottomBlob, &gemmBlob); // perform forward computation
	std::cout << "Starting conv test" << std::endl;
	fc2.Forward(&bottomBlob, &convBlob); // perform forward computation

	// check results
	bool testPassed = true;
	const float* gemmData = gemmBlob.getConstData();
	const float* convData = convBlob.getConstData();

	for(int dataIndex = 0; dataIndex < gemmBlob.count(); dataIndex++)
	{
		bool testPassed_temp = (gemmData[dataIndex] == convData[dataIndex]);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"FullyConnectedCompare",
					"Conv output %.1f incorrect at index = %i, gemm output = %.1f",
					convData[dataIndex], dataIndex, gemmData[dataIndex]
			);
		}
		testPassed &= testPassed_temp; // AND test into overall test result
	}

	std::string resultString = "\tFully Connected Layer Test ";
	resultString += (testPassed ? "PASSED\n" : "FAILED\n");
	std::cout << resultString;
	Logger::GetLogger()->LogMessage(resultString);

	return testPassed;
}
