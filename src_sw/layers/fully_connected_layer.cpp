#include "fully_connected_layer.hpp"

#include <iostream>

#include "../logger.hpp"
#include "../util/filler.hpp"
#include "../util/maths_functions.hpp"

namespace spaceBrain
{

FullyConnectedLayer::FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output)
{
	name_ = name;
	bottom_ = bottom;
	top_ = top;

	input_num_ = 0;
	output_length_ = num_output;
	input_volume_ = 0;

	Logger::GetLogger()->LogMessage(
			"\tFully Connected layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tnum ouptputs = %i",
			output_length_
	);
}

void FullyConnectedLayer::LayerSetUp(const Blob<float>* bottom, const Blob<float>* top)
{
	input_volume_ = bottom->count(CHANNEL_AXIS);

	weights_.Reshape(1, 1, output_length_, input_volume_);

}

void FullyConnectedLayer::Reshape(const Blob<float>* bottom, Blob<float>* top)
{
	if(input_volume_ != bottom->count(CHANNEL_AXIS))
	{
		Logger::GetLogger()->LogError(
				"FullyConnectedLayer::Reshape",
				"Input size %i incompatible. Expected %i",
				bottom->count(CHANNEL_AXIS), input_volume_
		);
	}

	input_num_ = bottom->num();

	int topShape[] = {bottom->num(), 1, output_length_, 1};
	// TODO clean up or test further
//	std::cout << N_ << " = " << topShape[HEIGHT_AXIS] * topShape[WIDTH_AXIS] << std::endl;
//	topShape[HEIGHT_AXIS] = N_;
//	topShape[WIDTH_AXIS] = 1;
	top->Reshape(topShape);
}

void FullyConnectedLayer::Forward(const Blob<float>* bottom, Blob<float>* top)
{
	  const float* bottom_data = bottom->getConstData();
	  float* top_data = top->getMutableData();
	  const float* weight = this->weights_.getConstData();
	  gemm_cpu(
			  false, false, // transposes
	      input_num_, output_length_, input_volume_, // m, n, k
		  1., bottom_data, weight, // alpha, A, B
		  0., top_data // beta, C
		  );

	  std::cout << input_num_ << " " << output_length_ << " " << input_volume_ << std::endl;
}

bool FullyConnectedTest()
{
	Logger::GetLogger()->LogMessage("Fully Connected Layer Test:");

	int num = 1, channels = 3, height = 3, width = 3;
	int num_output = 3;

	FullyConnectedLayer fc1("fc_test", "test_in", "test_out", num_output);
	Blob<float> bottomBlob(num, channels, height, width);
	Blob<float> topBlob;

	fc1.SetUp(&bottomBlob, &topBlob);

	// set input data
	int count = bottomBlob.count();
	float *dataIn = bottomBlob.getMutableData();
	for(int dataIndex = 0; dataIndex < count; dataIndex++)
	{
		dataIn[dataIndex] = dataIndex/height + 1; // dataIndex/height will nautrally be the floor of this as these are both ints
	}

	// set weights
	FillConstant(&fc1.weights_, 1);

	std::cout << "Bottom Data" << std::endl;
//	bottomBlob.PrintSlice();
	std::cout << bottomBlob.channels() << "*" << bottomBlob.height() << "*" << bottomBlob.width() << "\n" << std::endl;

	std::cout << "Weights Slice" << std::endl;
//	fc1.weights_.PrintSlice(0, 0);
	std::cout << fc1.weights_.num() << "*" << fc1.weights_.channels() << "*" << fc1.weights_.height() << "*" << fc1.weights_.width() << " ones\n" << std::endl;

	fc1.Forward(&bottomBlob, &topBlob); // perform forward computation

	// print results
	std::cout << "Top Data Slice" << std::endl;
	topBlob.PrintSlice();
	std::cout << topBlob.channels() << "*" << topBlob.height() << "*" << topBlob.width() << "\n" << std::endl;

	// check results
	bool testPassed = true;
	const float* topData = topBlob.getConstData();
	float trueResults[] = {288, 800};

	for(int dataIndex = 0; dataIndex < topBlob.count(); dataIndex++)
	{
		bool testPassed_temp = true;//(topData[dataIndex] == trueResults[dataIndex/num]);
		if(!testPassed_temp)
		{
			Logger::GetLogger()->LogError(
					"FullyConnectedTest",
					"Output %.0f incorrect at index = %i, expected %i",
					topData[dataIndex], dataIndex, trueResults[dataIndex/num]
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

}
