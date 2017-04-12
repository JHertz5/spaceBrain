/*
 * relu_layer.cpp
 *
 *  Created on: 11 Apr 2017
 *      Author: jjh113
 */

#include "relu_layer.hpp"
#include "stdlib.h"
#include <cmath>

namespace spaceBrain
{

ReluLayer::ReluLayer(std::string name, std::string bottom, std::string top)
{
	ReluLayer::name = name;
	ReluLayer::bottom = bottom;
	ReluLayer::top = top;
}

void ReluLayer::Forward(const Blob &bottom, const Blob &top)
{
	const float* bottomData = bottom.data;
	float* topData = top.data;
	const uint count = bottom.count();
	for (uint dataIndex = 0; dataIndex < count; ++dataIndex)
	{
		topData[dataIndex] = std::max(bottomData[dataIndex], (float)0);
	}
}





}


