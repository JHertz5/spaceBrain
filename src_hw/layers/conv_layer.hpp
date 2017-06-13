#ifndef SRC_LAYERS_CONV_LAYER_HPP_
#define SRC_LAYERS_CONV_LAYER_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"
#include "../blob.hpp"
#include "../layer.hpp"

class ConvolutionLayer : public Layer
{
public:

	ConvolutionLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride, int output_depth_);
	virtual ~ConvolutionLayer(){}

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top);

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);
	void Forward_gemm(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const int type() const
	{
		return CONV;
	}

	void ConvertBlobToInputColumns(const float* data_im, float* data_col);

	void conv_gemm_cpu(const float* input, const float* weights, float* output, bool skip_im2col = false);

	void Convolution(const float* input, const float* weights, float* output);

	void Forward_hw(const Blob<float>* bottom, Blob<float>* top);

	void Convolution_hw(const float* input, const float* weights, float* output);

	Blob<float> weights_;

private:
	Blob<float> col_buffer_;

	int kernel_size_;
	int stride_;
	int pad_;
	int num_input_;
	int output_depth_;
	int kernel_volume_; // volume of a single kernel, i.e. count on axes channel, height and width
	int output_spatial_volume_; // volume of output space,  i.e. count on axes height and width
	int output_size_;
	int input_depth_;
	int input_size_;

	bool is_1x1_;
};

bool ConvTest();

bool ConvHardwareTest();

void ConvSpeed();

bool ConvCompare();

#endif /* SRC_LAYERS_CONV_LAYER_HPP_ */
