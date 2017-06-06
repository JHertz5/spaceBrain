#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/iosfwd"
#include "/opt/Xilinx/SDx/2016.4/SDK/gnu/aarch32/lin/gcc-arm-linux-gnueabi/arm-linux-gnueabihf/include/c++/5.2.1/string"

#define MAX_LOG_MESSAGE_LENGTH 128

namespace spaceBrain
{

class Logger
{
public:

	void LogError(const char* Function, const char* format, ...);

	void LogMessage(const char* format, ...); //log variable message
	void LogMessage(const std::string &message); // log string

	static Logger* GetLogger();
	static void CloseLogger();

private:
	Logger(); // constructor

	static Logger* pThis; // pointer to logger class object
	static const std::string filename; // filename for log file
	static std::ofstream logStream; // output stream for log file

};

void LoggerTest();

}

#endif /* SRC_LOGGER_HPP_ */
