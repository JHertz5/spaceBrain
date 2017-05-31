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

	ConvolutionLayer(std::string name, std::string bottom, std::string top, int kernelSize,  int stride, int pad, int num_outputChannels);
	virtual ~ConvolutionLayer(){}

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top);

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top);

	virtual inline const char* type() const
	{
		return "Convolution";
	}

	void im2col(const float* data_im, const int channels,
			const int height, const int width,
			const int kernel_h, const int kernel_w,
		    const int pad_h, const int pad_w,
		    const int stride_h, const int stride_w,
		    const int dilation_h, const int dilation_w,
		    float* data_col);


private:
	Blob<float> weights_;
	Blob<float> col_buffer_;

	int kernel_size_;
	int stride_;
	int pad_;
	int num_output_channels_;
	int num_input_channels_;

	bool is_1x1_;

	int conved_size_;
	int channels_;
	int input_size_;
};

}

#endif /* SRC_LAYERS_CONV_LAYER_HPP_ */
