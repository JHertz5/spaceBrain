/*
 * logger.hpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#define MAX_LOG_MESSAGE_LENGTH 64

namespace spaceBrain
{

class Logger
{
public:

	void LogMessage(const std::string &message); // log string
	void LogMessage(const char* format, ...); //log variable message

	static Logger* GetLogger();
	static void CloseLogger();

private:
	Logger(); // constructor

	static Logger* pThis; // pointer to logger class object
	static const std::string filename; // filename for log file
	static std::ofstream logStream; // output stream for log file

};

}

#endif /* SRC_LOGGER_HPP_ */
