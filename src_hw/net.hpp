#ifndef SRC_NET_HPP_
#define SRC_NET_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/vector"
#include "blob.hpp"
#include "layer.hpp"

class Net
{
public:

	Net(std::string name, int inputNum, int inputDepth, int inputHeight, int inputWidth) : name_(name)
	{
		input_blob_ = new Blob<int>("data", inputNum, inputDepth, inputHeight, inputWidth);
		output_blob_ = new Blob<int>;
	};
	~Net(){}

	void Init();

	void SetLayers();

	void SetBlobs();

	void Reshape();

	void PrintLayerOutputShapes();

	void Forward();

	inline const std::string name() const
	{
		return name_;
	}

	inline Blob<int>* input_blob()
	{
		return input_blob_;
	}

	inline Blob<int>* output_blob()
	{
		return output_blob_;
	}

protected:

	std::string name_; // network name
	std::vector<Layer*> layers_;
	std::vector<Blob<int>* > blobs_;

	Blob<int>* input_blob_;
	Blob<int>* output_blob_;
};

void NetTest();

#endif /* SRC_NET_HPP_ */
