/*
 * dropout_layer.hpp
 *
 *  Created on: 18 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_LAYERS_DROPOUT_LAYER_HPP_
#define SRC_LAYERS_DROPOUT_LAYER_HPP_

#include <string>

#include "../layer.hpp"

namespace spaceBrain
{

class DropoutLayer : public Layer
{
public:

	DropoutLayer(std::string name, std::string bottom, std::string top);
	virtual ~DropoutLayer(){}

	virtual void Reshape(const Blob* bottom, Blob* top);

	virtual void Forward(const Blob *bottom, const Blob *top);

	virtual inline const char* type() const
	{
		return "Dropout";
	}

};

bool DropoutTest();

}

#endif /* SRC_LAYERS_DROPOUT_LAYER_HPP_ */
