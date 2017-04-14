/*
 * data_memory.hpp
 *
 *  Created on: 14 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_DATA_MEMORY_HPP_
#define SRC_DATA_MEMORY_HPP_

#include "stdlib.h"

namespace spaceBrain
{

class DataMemory
{
public:

	DataMemory();
//	DataMemory(size_t size);
	~DataMemory();

	void InitData(const size_t size);
	void SetData(void* dataPtr);

	const void* getConstData()
	{
		return (const void*) ptr_;
	}

	void* getMutableData()
	{
		return ptr_;
	}

	size_t size()
	{
		return size_;
	}

private:

	bool isPtrSet_;
	size_t size_;
	void* ptr_;
};

void DataMemoryTest();

}

#endif /* SRC_DATA_MEMORY_HPP_ */
