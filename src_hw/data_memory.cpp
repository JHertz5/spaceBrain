#include <data_memory.hpp>
#include <logger.hpp>
#include <stdlib.h>
#include <iostream>

namespace spaceBrain
{

DataMemory::DataMemory()
{
	isPtrSet_ = false;
	size_ = 0;
	ptr_ = NULL;
}

DataMemory::~DataMemory()
{
	if(ptr_ && isPtrSet_)
	{
		free(ptr_); // XXX maybe this changes to sds_free
	}
}

void DataMemory::InitData(const size_t size)
{
	if(size > 0) // check that isn't 0
	{
		size_ = size;
		if(ptr_ == NULL)
		{
			ptr_ = malloc(size_);
		}
		//std::memset(ptr_, 0, size_); // XXX Check that this doesn't cause problems
		isPtrSet_ = true;
	}
	else
	{
		Logger::GetLogger()->LogError(
				"ReluTest",
				"Attempt to InitData with size %i",
				size
		);
	}
}

void DataMemory::SetData(void* dataPtr)
{
	if(isPtrSet_)
	{
		free(ptr_); // XXX maybe this changes to sds_free
	}
	ptr_ = dataPtr;
	isPtrSet_ = true;
}

void DataMemoryTest()
{
	DataMemory data1;

	int num = 8;
	data1.InitData(num*sizeof(int));
	int* mut_dataPtr = (int*) data1.getMutableData();

	size_t dataLength = data1.size()/sizeof(int);
	for(size_t dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		mut_dataPtr[dataIndex] = dataIndex;//array[dataIndex];
	}

	std::cout << "data set" << std::endl;

	const int* dataPtr = (const int*) data1.getConstData();

	std::cout << "\tData size = " << dataLength << std::endl;
	std::cout << "\tData contents = { ";
	for(size_t dataIndex = 0; dataIndex < dataLength; dataIndex++)
	{
		std::cout << dataPtr[dataIndex] << " ";
	}
	std::cout << "}" << std::endl;

}

}


