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

	virtual void LayerSetUp(const Blob<int>* bottom, const Blob<int>* top);

	virtual void Reshape(const Blob<int>* bottom, Blob<int>* top);

	virtual void Forward(const Blob<int>* bottom, Blob<int>* top);
	void Forward_im2col(const Blob<int>* bottom, Blob<int>* top);

	virtual inline const int type() const
	{
		return CONV;
	}

	void ConvertBlobToInputColumns(const int* data_im, int* data_col);

	void conv_gemm_cpu(const int* input, const int* weights, int* output, bool skip_im2col = false);

	void Convolution(const int* input, const int* weights, int* output);
	void Convolution_hw(const int* input, const int* weights, int* output);

	void Forward_hw(const Blob<int>* bottom, Blob<int>* top);

	Blob<int> weights_;

private:
	Blob<int> col_buffer_;

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

bool ConvCompare();

}

#endif /* SRC_LAYERS_CONV_LAYER_HPP_ */
