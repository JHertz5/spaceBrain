/*
 * relu_layer.cpp
 *
 *  Created on: 11 Apr 2017
 *      Author: jjh113
 */

#include "relu_layer.hpp"

namespace spaceBrain
{

ReluLayer::ReluLayer(std::string name, std::string bottom, std::string top)
{
	ReluLayer::name = name;
	//ReluLayer::type = "ReLU";
	ReluLayer::bottom = bottom;
	ReluLayer::top = top;
}







}


