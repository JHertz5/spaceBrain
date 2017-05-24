#include "logger.hpp"

#include <stdio.h>
#include <cstdarg>
#include <fstream>

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

void Logger::LogError(const char* functionName, const char* format, ...)
{
	const int errorMessageMaxLength = MAX_LOG_MESSAGE_LENGTH - 12; // Error message formatting adds 12 characters

	va_list args;
	va_start(args, format);

	char* messageChars = new char[errorMessageMaxLength]; // allocate Cstring space
	vsnprintf(messageChars, errorMessageMaxLength, format, args); // write message to Cstring

	va_end(args);

	LogMessage("Error: %s() - %s", functionName, messageChars);
}

void Logger::LogMessage(const char* format, ...)
{
    char* messageChars = NULL;

    va_list args;
    va_start(args, format);

    messageChars = new char[MAX_LOG_MESSAGE_LENGTH]; // allocate Cstring space
    vsnprintf(messageChars, MAX_LOG_MESSAGE_LENGTH, format, args); // write message to Cstring

    va_end(args);

    std::string messageString(messageChars); // initialise string with Cstring
    delete [] messageChars;

    LogMessage(messageString); // log string
}

void Logger::LogMessage(const std::string &message)
{
	logStream << message << std::endl;
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
		logStream << "END LOG" << std::endl;
		pThis = NULL;
		logStream.close();
	}
}

}
