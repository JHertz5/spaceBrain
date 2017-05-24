#ifndef SRC_LAYERS_POOLING_LAYER_HPP_
#define SRC_LAYERS_POOLING_LAYER_HPP_

#include <string>

#include "../blob.hpp"
#include "../layer.hpp"

namespace spaceBrain
{

class PoolingLayer : public Layer
{
public:

	PoolingLayer(std::string name, std::string bottom, std::string top, int pad, int kernelSize, int stride);
	virtual ~PoolingLayer(){}

	virtual void LayerSetUp(const Blob* bottom, const Blob* top);

	virtual void Reshape(const Blob* bottom, Blob* top);

	virtual void Forward(const Blob *bottom, const Blob *top);

	virtual inline const char* type() const
	{
		return "Pooling";
	}

protected:
	 // pad, kernelSize, and pooledSize are both height and width as only square blobs are supported
	int pad_;
	int kernelSize_;
	int stride_;
	int pooledSize_;
	int channels_, height_, width_;

};

bool PoolTest();

}

#endif /* SRC_LAYERS_POOLING_LAYER_HPP_ */
