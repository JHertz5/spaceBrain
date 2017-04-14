/*
 * data_memory.cpp
 *
 *  Created on: 14 Apr 2017
 *      Author: jhertz5
 */

#include "data_memory.hpp"
#include <cstring>
#include <iostream>

namespace spaceBrain
{
/*
DataMemory::DataMemory()
{
	isPtrSet_ = false;
	size_ = 0;
	ptr_ = NULL;
}
*/
DataMemory::DataMemory(size_t size)
{
	isPtrSet_ = false;
	size_ = size;
	ptr_ = NULL;
}

DataMemory::~DataMemory()
{
	/*
	if(ptr_ != NULL && isPtrSet_)
	{
		free(ptr_); // XXX maybe this changes to sds_free
	}
	*/
}
/*
void DataMemory::InitData(size_t size)
{
	size_ = size;
	if(ptr_ == NULL)
	{
		ptr_ = malloc(size_);
	}
	std::memset(ptr_, 0, size_);
	isPtrSet_ = true;
}
 */
void DataMemory::SetData(void* dataPtr)
{
	/*
	if(isPtrSet_)
	{
		free(ptr_); // XXX maybe this changes to sds_free
	}
	*/
	ptr_ = dataPtr;
	isPtrSet_ = true;
}

void DataMemoryTest()
{
	spaceBrain::DataMemory data1(8);
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8};

	data1.SetData((void*) array);
	const int* dataPtr = (const int*) data1.getConstData();

	std::cout << "\tData size = " << data1.size() << std::endl;
	std::cout << "\tData contents = { ";
	for(size_t dataIndex = 0; dataIndex < data1.size(); dataIndex++)
	{
		std::cout << dataPtr[dataIndex] << " ";
	}
	std::cout << "}" << std::endl;


}

}


