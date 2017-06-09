#ifndef SRC_NET_HPP_
#define SRC_NET_HPP_

#include <string>
#include <vector>
#include <memory>

#include "blob.hpp"
#include "layer.hpp"

namespace spaceBrain
{

class Net
{
public:

	Net() : name_("testNet"){};
	~Net(){}

	void Init();

	inline const std::string name() const
	{
		return name_;
	}

protected:

	std::string name_; // network name
	std::vector<Layer*> layers_;
	std::vector<std::string> layer_names_;
	std::vector<Blob<float>* > blobs_;
	std::vector<std::string> blob_names_;
};

}

#endif /* SRC_NET_HPP_ */
