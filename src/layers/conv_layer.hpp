#ifndef SRC_LAYERS_CONV_LAYER_HPP_
#define SRC_LAYERS_CONV_LAYER_HPP_

#include <string>

#include "../blob.hpp"
#include "../layer.hpp"

namespace spaceBrain
{

class ConvolutionLayer : public Layer
{
public:

	ConvolutionLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride, int num_output);
	virtual ~ConvolutionLayer(){}

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top);

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "Convolution";
	}

	void im2col(const float* data_im, float* data_col); // TODO give better name

	void forward_cpu_gemm(const float* input, const float* weights, float* output, bool skip_im2col = false); // TODO rename to fit convention

	Blob<float> col_buffer_; // TODO set back to private

private:
	Blob<float> weights_;

	int kernel_size_;
	int stride_;
	int pad_;
	int number_of_kernels;
	int kernel_volume_;
	int output_spatial_volume_;
	int output_size_;
	int channels_;
	int input_size_;

	bool is_1x1_;
};

bool ConvTest();

}

#endif /* SRC_LAYERS_CONV_LAYER_HPP_ */
