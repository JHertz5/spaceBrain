#ifndef SRC_HW_UTIL_TIMER_HPP_
#define SRC_HW_UTIL_TIMER_HPP_

#include "/opt/Xilinx/SDx/2016.4/aarch32-linux/include/sds_lib.h"

namespace space_brain
{

class timer
{
public:

	timer() : total_cycles_(0), count_(0), num_calls_(0) {};

	inline void reset()
	{
		total_cycles_ = count_ = num_calls_ = 0;
	}

	inline void start()
	{
		count_ = sds_clock_counter();
		num_calls_++;
	};

	inline void stop()
	{
		total_cycles_ += (sds_clock_counter() - count_);
	};

	inline int getAverageCpuTime()
	{
		return (total_cycles_ / num_calls_);
	};

private:
	int total_cycles_, count_, num_calls_;
};

}

#endif /* SRC_HW_UTIL_TIMER_HPP_ */
