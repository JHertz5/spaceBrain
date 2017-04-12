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
	std::string name; // the layer name
	std::string bottom; // the name of each bottom blob
	std::string top; // the name of each top blob

	virtual void Forward(const Blob &bottom, const Blob &top) = 0;

	virtual inline const char* type() const
	{
		return "";
	}
};

}

#endif /* SRC_LAYERS_LAYER_HPP_ */
