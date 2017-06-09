#ifndef SRC_UTIL_IO_HPP_
#define SRC_UTIL_IO_HPP_

namespace spaceBrain
{

void printHelpMessage(bool commandLineVersion);
void performCommand(std::string inMessage, bool* keepRunning);

}

#endif /* SRC_UTIL_IO_HPP_ */
