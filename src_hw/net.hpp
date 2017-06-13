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
		input_blob_ = new Blob<float>("data", inputNum, inputDepth, inputHeight, inputWidth);
		output_blob_ = new Blob<float>;
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

	inline Blob<float>* input_blob()
	{
		return input_blob_;
	}

	inline Blob<float>* output_blob()
	{
		return output_blob_;
	}

protected:

	std::string name_; // network name
	std::vector<Layer*> layers_;
	std::vector<Blob<float>* > blobs_;

	Blob<float>* input_blob_;
	Blob<float>* output_blob_;
};

void NetTest();

#endif /* SRC_NET_HPP_ */
