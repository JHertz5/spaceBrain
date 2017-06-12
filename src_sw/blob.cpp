#include "blob.hpp"

#include <cstring>

#include "logger.hpp"

namespace spaceBrain
{

template <typename Dtype>
Blob<Dtype>::Blob()
{
	name_ = "NAMELESS";

	count_ = 0;
	data_ = new DataMemory();
	shape_[NUM_AXIS] = 0;
	shape_[DEPTH_AXIS] = 0;
	shape_[HEIGHT_AXIS] = 0;
	shape_[WIDTH_AXIS] = 0;
	count_ = 0;

	Logger::GetLogger()->LogMessage("\tBlob constructed with no data");
}

template <typename Dtype>
Blob<Dtype>::Blob(std::string name)
{
	name_ = name;

	count_ = 0;
	data_ = new DataMemory();
	shape_[NUM_AXIS] = 0;
	shape_[DEPTH_AXIS] = 0;
	shape_[HEIGHT_AXIS] = 0;
	shape_[WIDTH_AXIS] = 0;
	count_ = 0;

	Logger::GetLogger()->LogMessage("\tBlob constructed with no data");
}

template <typename Dtype>
Blob<Dtype>::Blob(std::string name, const int num, const int depth, const int height, const int width)
{
	name_ = name;

	count_ = 0;
	data_ = new DataMemory();
	this->Reshape(num, depth, height, width);

	Logger::GetLogger()->LogMessage("\tBlob constructed with data shape = (%i*%i*%i*%i)", num, depth, height, width);
}

template <typename Dtype>
Blob<Dtype>::Blob(const int num, const int depth, const int height, const int width)
{
	name_ = "NAMELESS";

	count_ = 0;
	data_ = new DataMemory();
	this->Reshape(num, depth, height, width);

	Logger::GetLogger()->LogMessage("\tBlob constructed with data shape = (%i*%i*%i*%i)", num, depth, height, width);
}

template <typename Dtype>
Blob<Dtype>::~Blob()
{
	if(data_ != NULL)
	{
		delete data_; // XXX change to hardware sds_free
	}
}

template <typename Dtype>
void Blob<Dtype>::Reshape(const int num, const int depth, const int height, const int width)
{
	int shape[NUM_BLOB_DIMENSIONS];
	shape[NUM_AXIS] = num;
	shape[DEPTH_AXIS] = depth;
	shape[HEIGHT_AXIS] = height;
	shape[WIDTH_AXIS] = width;
	Reshape(shape);
}

template <typename Dtype>
void Blob<Dtype>::Reshape(const int shapeIn[NUM_BLOB_DIMENSIONS])
{
	int count = 1;
	for (int dimensionIndex = 0; dimensionIndex < NUM_BLOB_DIMENSIONS; dimensionIndex++) {
		count *= shapeIn[dimensionIndex];
		this->shape_[dimensionIndex] = shapeIn[dimensionIndex];
	}

	if (count != this->count_)
	{
		Logger::GetLogger()->LogMessage("\tBlob::Reshape: count %i->%i", count_, count);
		count_ = count;
		data_->InitData(count_ * sizeof(Dtype));
	}
}

template <typename Dtype>
void Blob<Dtype>::ReshapeLike(const Blob<Dtype> &thatBlob)
{
	Reshape(thatBlob.shape());
}

template <typename Dtype>
void Blob<Dtype>::SetData(const Dtype* dataIn, const int countIn)
{
	data_->SetData((void*) dataIn);
}

template <typename Dtype>
const Dtype* Blob<Dtype>::getConstData() const
{
	return (const Dtype*) data_->getConstData();
}

template <typename Dtype>
Dtype* Blob<Dtype>::getMutableData() const
{
	return (Dtype*) data_->getMutableData();
}

template <typename Dtype>
void Blob<Dtype>::CopyFrom(const Blob<Dtype>* source, bool reshape)
{
	// check that sizes match, reshape if not
	if (source->count() != count_ || source->shape() != shape_)
	{
		if (reshape)
		{
			ReshapeLike(*source);
		}
		else
		{
			Logger::GetLogger()->LogError(
					"Blob::CopyFrom",
					"Source shape does not match current shape but reshape == false"
			);
			return;
		}
	}

	const float* sourceDataArray = (float*) source->getConstData();
	float* thisDataArray = (float*) data_->getMutableData();

	for(int index = 0; index < this->count(); index++)
	{
		thisDataArray[index] = sourceDataArray[index];
	}
}

template <typename Dtype>
void Blob<Dtype>::PrintSlice(const int num, const int depth)
{
	if(num >= this->num())
	{
		Logger::GetLogger()->LogError("Blob::PrintSlice", "slice num %i >= blob layers %i", num, this->num());
		return;
	}
	if(depth >= this->depth())
	{
		Logger::GetLogger()->LogError("Blob::PrintSlice", "slice depth %i >= blob depth %i", depth, this->depth());
		return;
	}

	const Dtype* data = this->getConstData();

	for(int hIndex = 0; hIndex < this->height(); hIndex++)
	{
		for(int wIndex = 0; wIndex < this->width(); wIndex++)
		{
			std::cout << data[offset(num, depth, hIndex, wIndex)] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

}

template <typename Dtype>
void Blob<Dtype>::PrintShape()
{
	std::cout << "["
				<< width() << "*"
				<< height() << "*"
				<< depth() << "*"
				<< num()
				<< "]\t" << name() << std::endl;
}

//explicit instantiation
template class Blob<float>;
template class Blob<int>;

} // end of namespace spaceBrain
