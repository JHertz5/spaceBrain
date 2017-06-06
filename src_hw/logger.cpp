#include <logger.hpp>
#include <stdio.h>
#include <cstdarg>
#include <iostream>

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

	char messageChars[errorMessageMaxLength];
	vsnprintf(messageChars, errorMessageMaxLength, format, args); // write message to Cstring

	va_end(args);

	LogMessage("Error: %s() - %s", functionName, messageChars);
}

void Logger::LogMessage(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char messageChars[MAX_LOG_MESSAGE_LENGTH];
    vsnprintf(messageChars, MAX_LOG_MESSAGE_LENGTH, format, args); // write message to Cstring

    va_end(args);

    std::string messageString(messageChars); // initialise string with Cstring

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

void LoggerTest()
{
	Logger::GetLogger()->LogMessage("LoggerTest");

	std::cin.ignore();
	std::cout << "\tinput log message (one line):" << std::endl;

	char message[256];
	std::cin.getline(message, sizeof(message));

	spaceBrain::Logger::GetLogger()->LogMessage(message);

	std::cout << std::endl;
}

}
