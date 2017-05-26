#ifndef SRC_UTIL_FILLER_HPP_
#define SRC_UTIL_FILLER_HPP_

#include "../blob.hpp"

namespace spaceBrain
{

void FillConstant(Blob* blob, float value);
void FillUniform(Blob* blob, int min, int max);
bool FillerTest();

}

#endif /* SRC_UTIL_FILLER_HPP_ */
