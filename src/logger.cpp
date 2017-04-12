/*
 * logger.cpp
 *
 *  Created on: 12 Apr 2017
 *      Author: jhertz5
 */

#include "logger.hpp"
#include <cstdarg>
#include <stdio.h>

namespace spaceBrain
{

// static Logger variables declared
const std::string Logger::filename = "space_brain.log";
Logger* Logger::pThis = NULL;
std::ofstream Logger::logStream;

// constructor doesn't need to do anything
Logger::Logger()
{

}

void Logger::LogMessage(const std::string &message)
{
	logStream << message << std::endl;
}

void Logger::LogMessage(const char* format, ...)
{
    char* messageString = NULL;

    va_list args;
    va_start(args, format);

    messageString = new char[MAX_LOG_MESSAGE_LENGTH]; // allocate string space
    vsnprintf(messageString, MAX_LOG_MESSAGE_LENGTH, format, args); // write message to string
//    logStream << messageString << "\n"; // write string to log stream

    va_end(args);

    delete [] messageString;

    LogMessage(messageString);
}

Logger* Logger::GetLogger()
{
	if(pThis == NULL)
	{
		pThis = new Logger();
		logStream.open(filename.c_str());
	}
	return pThis;
}

void Logger::CloseLogger()
{
	if(pThis != NULL || logStream.is_open())
	{
		pThis = NULL;
		logStream.close();
	}
}

}
