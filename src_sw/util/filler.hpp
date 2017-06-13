#ifndef SRC_UTIL_FILLER_HPP_
#define SRC_UTIL_FILLER_HPP_

#include "../blob.hpp"

namespace spaceBrain
{

/*
 * Fills blob with constant value
 * param: blob - blob to be filled
 * param: value - constant value to set blob data to
 */
void FillConstant(Blob<int>* blob, int value);
void FillConstant(int* data, int dataLength, int value);

/*
 * Fills blob with uniformly distributed values
 * param: blob - blob to be filled
 * param: min - minimum value for uniform distribution
 * param: max - maximum value for uniform distribution
 */
void FillUniform(Blob<int>* blob, int min, int max);
void FillUniform(int* data, int dataLength, int min, int max);

/*
 * Test Filler functions
 * return: true if all filled values are valid
 */
bool FillerTest();

}

#endif /* SRC_UTIL_FILLER_HPP_ */
