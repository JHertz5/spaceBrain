#include "fully_connected_layer.hpp"

#include "../logger.hpp"
#include "../util/filler.hpp"

namespace spaceBrain
{

FullyConnectedLayer::FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output)
{
	num_output_ = num_output;

	input_volume_ = 0;

	Logger::GetLogger()->LogMessage(
			"\tFully Connected layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tnum ouptputs = %i",
			num_output_
	);
}

void FullyConnectedLayer::LayerSetUp(const Blob<float>* bottom, const Blob<float>* top)
{
	int channelAxis = CHANNELS;
	input_volume_ = bottom->count(channelAxis);

	weights_.Reshape(1, 1, num_output_, input_volume_);

}

}
