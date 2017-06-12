#ifndef SRC_LAYERS_LAYER_HPP_
#define SRC_LAYERS_LAYER_HPP_

#include <string>

#include "blob.hpp"

enum LayerType
{
	CONV,
	FC,
	RELU,
	POOL,
	DROPOUT,
	SOFTMAX
};

namespace spaceBrain
{

// Class from which to derive other layer classes
class Layer
{
public:

	void SetUp(const Blob<float>* bottom, Blob<float>* top)
	{
	    LayerSetUp(bottom, top);
	    Reshape(bottom, top);
	}

	virtual void LayerSetUp(const Blob<float>* bottom, const Blob<float>* top){}

	virtual void Reshape(const Blob<float>* bottom, Blob<float>* top) = 0;

	virtual void Forward(const Blob<float>* bottom, Blob<float>* top) = 0;

	virtual inline const int type() const = 0;

	virtual inline const std::string name() const
	{
		return name_;
	}

	virtual inline const std::string bottom() const
	{
		return bottom_;
	}

	virtual inline const std::string top() const
	{
		return top_;
	}


protected:
	std::string name_; // the layer name
	std::string bottom_; // the name of each bottom blob
	std::string top_; // the name of each top blob

	~Layer(){}
};

}

#endif /* SRC_LAYERS_LAYER_HPP_ */
