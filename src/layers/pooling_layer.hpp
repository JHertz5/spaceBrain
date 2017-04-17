/*
 * pooling_layer.hpp
 *
 *  Created on: 13 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_LAYERS_POOLING_LAYER_HPP_
#define SRC_LAYERS_POOLING_LAYER_HPP_

#include <string>

#include "../layer.hpp"

namespace spaceBrain
{

class PoolingLayer : public Layer
{
public:

	PoolingLayer(std::string name, std::string bottom, std::string top);
	virtual ~PoolingLayer(){}

	virtual void Forward(const Blob *bottom, const Blob *top);

	virtual inline const char* type() const
	{
		return "Pooling";
	}
};

bool PoolingTest();

}

#endif /* SRC_LAYERS_POOLING_LAYER_HPP_ */
