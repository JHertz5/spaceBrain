#include <iostream>
#include <string>

#include "logger.hpp"
#include "util/io.hpp"

int main(int argc, char* argv[])
{
	bool keepRunning = false;

	for (int argIndex = 1; argIndex < argc; argIndex++)
	{
		if(argv[argIndex][0] == '-')
		{
			switch(argv[argIndex][1])
			{
			case 'h':
				spaceBrain::printHelpMessage(true);
				return 0;

			case 't':
				keepRunning = true;
				std::cout << "Testing Interface Open" << std::endl;
				break;

			default:
				std::cout << "COMMAND NOT RECOGNISED" << std::endl;
			}
		}
		else
		{
			spaceBrain::performCommand(std::string(argv[argIndex]), &keepRunning);
		}
	}

	std::string inMessage;

	while(keepRunning)
	{
		std::cout << "Ready for input: (input 'help' for test commands)" << std::endl;
		std::cin >> inMessage;

		// respond to command
		spaceBrain::performCommand(inMessage, &keepRunning);
	} // end of while

	spaceBrain::Logger::CloseLogger();

}
