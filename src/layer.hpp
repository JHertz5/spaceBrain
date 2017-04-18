/*
 * layer.hpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_LAYERS_LAYER_HPP_
#define SRC_LAYERS_LAYER_HPP_

#include <string>

namespace spaceBrain {
class Blob;
} /* namespace spaceBrain */

namespace spaceBrain
{

// Class from which to derive other layer classes
class Layer
{
public:

	void SetUp(const Blob* bottom, Blob* top)
	{
	    LayerSetUp(bottom, top);
	    Reshape(bottom, top);
	}

	virtual void LayerSetUp(const Blob* bottom, const Blob* top){}

	virtual void Reshape(const Blob* bottom, Blob* top) = 0;

	virtual void Forward(const Blob* bottom, const Blob* top) = 0;

	virtual inline const char* type() const
	{
		return "";
	}

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
