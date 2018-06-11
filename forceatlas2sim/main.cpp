#include <iostream>
#include <string>

#include "fileparser.h"

int main(int argc, char** argv)
{
	std::string help = "Usage: forceatlas2sim.exe (-i INPUT) [-h]\n\n"\
		"GPU-based parallelization and graphical simulation of the ForceAtlas2 algorithm\n\n"\
		"Options:\n"\
		"-i INPUT   Input GEFX or GML graph file. Required.\n"\
		"-h         Prints usage.\n";

	std::string inputFile;

	switch (argc)
	{
	case 1:
		std::cout << help;
		return 0;
	case 2:
		if (argv[1] == std::string("-h"))
		{
			std::cout << help;
			return 0;
		}
	case 3:
		if (argv[1] == std::string("-i"))
		{
			inputFile = argv[2];
			break;
		}
	default:
		std::cout << "Invalid arguments. ";
		std::cout << help;
		return 1;
	}

	FileParser fileParser;
	fileParser.parse(inputFile);

	return 0;
}
