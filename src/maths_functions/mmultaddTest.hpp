/*
 * mmultaddTest.hpp
 *
 *  Created on: 6 Apr 2017
 *      Author: jjh113
 */

#ifndef SRC_MATHS_FUNCTIONS_MMULTADDTEST_HPP_
#define SRC_MATHS_FUNCTIONS_MMULTADDTEST_HPP_

#include "sds_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NUM_TESTS
#define NUM_TESTS 1024
#endif

class perf_counter
{
public:
	uint64_t tot, cnt, calls;
	perf_counter() : tot(0), cnt(0), calls(0) {};
	inline void reset() { tot = cnt = calls = 0; }
	inline void start() { cnt = sds_clock_counter(); calls++; };
	inline void stop() { tot += (sds_clock_counter() - cnt); };
	inline uint64_t avg_cpu_cycles() { return ((tot+(calls>>1)) / calls); };

	static void init_arrays(float *A,  float *B, float *C, float *D, float *D_sw);
	static int result_check(float *D, float *D_sw);
};

int mmult_test(float *A,  float *B, float *C, float *D, float *D_sw);

#endif /* SRC_MATHS_FUNCTIONS_MMULTADDTEST_HPP_ */
