#include "commands.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	// Flush after every std::cout / std::cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	if (argc < 2)
	{
		std::cerr << "No command provided.\n";
		return EXIT_FAILURE;
	}

	if (std::string command = argv[1]; command == "init")
	{
		return init();
	}
	else if (command == "cat-file")
	{
		if (argc < 4)
		{
			std::cerr << "Invalid arguments, `-p <blob_sha>` required.\n";
			return EXIT_FAILURE;
		}

		if (const std::string flag = argv[2]; flag != "-p")
		{
			std::cerr << "Invalid flag for cat-file, `-p` expected\n";
			return EXIT_FAILURE;
		}

		const std::string value = argv[3];
		return cat_file(value);
	}
	else
	{
		std::cerr << "Unknown command " << command << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
