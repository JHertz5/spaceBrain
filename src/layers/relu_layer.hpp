/*
 * relu_layer.hpp
 *
 *  Created on: 11 Apr 2017
 *      Author: jjh113
 */

#ifndef SRC_LAYERS_RELU_LAYER_HPP_
#define SRC_LAYERS_RELU_LAYER_HPP_

class ReluLayer
{
	std::string name, bottom, top;

	inline const char* type() const
	{
		return "ReLU";
	}

	//ReluLayer(std::string name, more params);

};



#endif /* SRC_LAYERS_RELU_LAYER_HPP_ */
