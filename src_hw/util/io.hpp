#ifndef SRC_UTIL_IO_HPP_
#define SRC_UTIL_IO_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"

void printHelpMessage(bool commandLineVersion);
void performCommand(std::string inMessage, bool* keepRunning);

#endif /* SRC_UTIL_IO_HPP_ */
