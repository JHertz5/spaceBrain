/*
 * layer.hpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_LAYERS_LAYER_HPP_
#define SRC_LAYERS_LAYER_HPP_

#include <string>
#include "blob.hpp"

namespace spaceBrain
{

// Class from which to derive other layer classes
class Layer
{
public:

	virtual void Forward(const Blob &bottom, const Blob &top) = 0;

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

};

}

#endif /* SRC_LAYERS_LAYER_HPP_ */
