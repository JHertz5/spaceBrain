#ifndef SRC_DATA_MEMORY_HPP_
#define SRC_DATA_MEMORY_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/lib/gcc/arm-linux-gnueabihf/5.2.1/include/stddef.h"

class DataMemory
{
public:

	DataMemory();
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

#endif /* SRC_DATA_MEMORY_HPP_ */
