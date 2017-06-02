#include "fully_connected_layer.hpp"

namespace spaceBrain
{

FullyConnectedLayer::FullyConnectedLayer(std::string name, std::string bottom, std::string top, int num_output)
{
	num_output_ = num_output;

	Logger::GetLogger()->LogMessage(
			"\tFully Connected layer '%s' constructed with bottom = '%s' and top = '%s'",
			name.c_str(), bottom.c_str(), top.c_str()
	);
	Logger::GetLogger()->LogMessage(
			"\t\tnum ouptputs = %i",
			num_output_
	);
}


}
